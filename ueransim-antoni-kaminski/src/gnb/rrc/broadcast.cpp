//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#include "task.hpp"

#include <gnb/ngap/task.hpp>
#include <lib/asn/rrc.hpp>
#include <lib/asn/utils.hpp>
#include <lib/rrc/encode.hpp>
#include <utils/common.hpp>

#include <asn/rrc/ASN_RRC_MIB.h>
#include <asn/rrc/ASN_RRC_PLMN-IdentityInfo.h>
#include <asn/rrc/ASN_RRC_PLMN-IdentityInfoList.h>
#include <asn/rrc/ASN_RRC_SIB1.h>
#include <asn/rrc/ASN_RRC_UAC-BarringInfoSet.h>
#include <asn/rrc/ASN_RRC_UAC-BarringInfoSetIndex.h>
#include <asn/rrc/ASN_RRC_UAC-BarringPerCat.h>
#include <asn/rrc/ASN_RRC_UAC-BarringPerCatList.h>
#include <asn/rrc/ASN_RRC_SystemInformation.h>
#include <asn/rrc/ASN_RRC_SystemInformation-IEs.h>
#include <asn/rrc/ASN_RRC_SIB2.h>


namespace nr::gnb
{

static ASN_RRC_BCCH_BCH_Message *ConstructMibMessage(bool barred, bool intraFreqReselectAllowed)
{
    auto *pdu = asn::New<ASN_RRC_BCCH_BCH_Message>();
    pdu->message.present = ASN_RRC_BCCH_BCH_MessageType_PR_mib;
    pdu->message.choice.mib = asn::New<ASN_RRC_MIB>();

    auto &mib = *pdu->message.choice.mib;

    asn::SetBitStringInt<6>(0, mib.systemFrameNumber);
    mib.subCarrierSpacingCommon = ASN_RRC_MIB__subCarrierSpacingCommon_scs15or60;
    mib.ssb_SubcarrierOffset = 0;
    mib.dmrs_TypeA_Position = ASN_RRC_MIB__dmrs_TypeA_Position_pos2;
    mib.cellBarred = barred ? ASN_RRC_MIB__cellBarred_barred : ASN_RRC_MIB__cellBarred_notBarred;
    mib.intraFreqReselection = intraFreqReselectAllowed ? ASN_RRC_MIB__intraFreqReselection_allowed
                                                        : ASN_RRC_MIB__intraFreqReselection_notAllowed;
    asn::SetBitStringInt<1>(0, mib.spare);
    mib.pdcch_ConfigSIB1.controlResourceSetZero = 0;
    mib.pdcch_ConfigSIB1.searchSpaceZero = 0;
    return pdu;
}

static ASN_RRC_BCCH_DL_SCH_Message *ConstructSib1Message(bool cellReserved, int tac, int64_t nci, const Plmn &plmn,
                                                         const UacAiBarringSet &aiBarringSet, const Sib1Config &sib1conf)
{
    auto *pdu = asn::New<ASN_RRC_BCCH_DL_SCH_Message>();
    pdu->message.present = ASN_RRC_BCCH_DL_SCH_MessageType_PR_c1;
    pdu->message.choice.c1 = asn::NewFor(pdu->message.choice.c1);
    pdu->message.choice.c1->present = ASN_RRC_BCCH_DL_SCH_MessageType__c1_PR_systemInformationBlockType1;
    pdu->message.choice.c1->choice.systemInformationBlockType1 = asn::New<ASN_RRC_SIB1>();

    auto &sib1 = *pdu->message.choice.c1->choice.systemInformationBlockType1;

    auto cellSelectInfo = asn::New<ASN_RRC_SIB1::ASN_RRC_SIB1__cellSelectionInfo>();
    cellSelectInfo->q_RxLevMin = sib1conf.q_RxLevMin;
    if (sib1conf.q_RxLevMinOffset) {
        asn::MakeNew(cellSelectInfo->q_RxLevMinOffset);
        *cellSelectInfo->q_RxLevMinOffset = *sib1conf.q_RxLevMinOffset;
    }
    if (sib1conf.q_RxLevMinSUL) {
        asn::MakeNew(cellSelectInfo->q_RxLevMinSUL);
        *cellSelectInfo->q_RxLevMinSUL = *sib1conf.q_RxLevMinSUL;
    }
    if (sib1conf.q_QualMin) {
        asn::MakeNew(cellSelectInfo->q_QualMin);
        *cellSelectInfo->q_QualMin = *sib1conf.q_QualMin;
    }
    if (sib1conf.q_QualMinOffset) {
        asn::MakeNew(cellSelectInfo->q_QualMinOffset);
        *cellSelectInfo->q_QualMinOffset = *sib1conf.q_QualMinOffset;
    }
    sib1.cellSelectionInfo = cellSelectInfo;


    if (cellReserved)
    {
        asn::MakeNew(sib1.cellAccessRelatedInfo.cellReservedForOtherUse);
        *sib1.cellAccessRelatedInfo.cellReservedForOtherUse =
            ASN_RRC_CellAccessRelatedInfo__cellReservedForOtherUse_true;
    }

    auto *plmnInfo = asn::New<ASN_RRC_PLMN_IdentityInfo>();
    plmnInfo->cellReservedForOperatorUse = cellReserved
                                               ? ASN_RRC_PLMN_IdentityInfo__cellReservedForOperatorUse_reserved
                                               : ASN_RRC_PLMN_IdentityInfo__cellReservedForOperatorUse_notReserved;
    asn::MakeNew(plmnInfo->trackingAreaCode);
    asn::SetBitStringInt<24>(tac, *plmnInfo->trackingAreaCode);
    asn::SetBitStringLong<36>(nci, plmnInfo->cellIdentity);
    asn::SequenceAdd(plmnInfo->plmn_IdentityList, asn::rrc::NewPlmnId(plmn));
    asn::SequenceAdd(sib1.cellAccessRelatedInfo.plmn_IdentityList, plmnInfo);

    asn::MakeNew(sib1.uac_BarringInfo);

    auto *info = asn::New<ASN_RRC_UAC_BarringInfoSet>();
    info->uac_BarringFactor = ASN_RRC_UAC_BarringInfoSet__uac_BarringFactor_p50;
    info->uac_BarringTime = ASN_RRC_UAC_BarringInfoSet__uac_BarringTime_s4;

    asn::SetBitStringInt<7>(bits::Consequential8(false, aiBarringSet.ai1, aiBarringSet.ai2, aiBarringSet.ai11,
                                                 aiBarringSet.ai12, aiBarringSet.ai13, aiBarringSet.ai14,
                                                 aiBarringSet.ai15),
                            info->uac_BarringForAccessIdentity);

    asn::SequenceAdd(sib1.uac_BarringInfo->uac_BarringInfoSetList, info);

    asn::MakeNew(sib1.uac_BarringInfo->uac_BarringForCommon);

    for (size_t i = 0; i < 63; i++)
    {
        auto *item = asn::New<ASN_RRC_UAC_BarringPerCat>();
        item->accessCategory = static_cast<decltype(item->accessCategory)>(i + 1);
        item->uac_barringInfoSetIndex = 1;

        asn::SequenceAdd(*sib1.uac_BarringInfo->uac_BarringForCommon, item);
    }

    return pdu;
}

static ASN_RRC_BCCH_DL_SCH_Message *ConstructSib2Message(const Sib2Config &sib2conf) {
    auto *pdu = asn::New<ASN_RRC_BCCH_DL_SCH_Message>();
    pdu->message.present = ASN_RRC_BCCH_DL_SCH_MessageType_PR_c1;
    pdu->message.choice.c1 = asn::NewFor(pdu->message.choice.c1);
    pdu->message.choice.c1->present = ASN_RRC_BCCH_DL_SCH_MessageType__c1_PR_systemInformation;
    pdu->message.choice.c1->choice.systemInformation = asn::New<ASN_RRC_SystemInformation>();
    
    auto &si = *pdu->message.choice.c1->choice.systemInformation;
    si.criticalExtensions.present = ASN_RRC_SystemInformation__criticalExtensions_PR_systemInformation;
    si.criticalExtensions.choice.systemInformation = asn::New<ASN_RRC_SystemInformation_IEs>();
    
    auto *si_ies = si.criticalExtensions.choice.systemInformation;
    auto *ie_sib2 = asn::New<ASN_RRC_SystemInformation_IEs__sib_TypeAndInfo__Member>();
    ie_sib2->present = ASN_RRC_SystemInformation_IEs__sib_TypeAndInfo__Member_PR_sib2;
    ie_sib2->choice.sib2 = asn::New<ASN_RRC_SIB2>();

    auto &sib2 = *ie_sib2->choice.sib2;
    sib2.cellReselectionInfoCommon.q_Hyst = sib2conf.q_Hyst;
    sib2.cellReselectionServingFreqInfo.threshServingLowP = sib2conf.treshServingLowP;
    sib2.cellReselectionServingFreqInfo.cellReselectionPriority = sib2conf.cellReselectionPriority;
    sib2.intraFreqCellReselectionInfo.q_RxLevMin = sib2conf.q_RxLevMin;
    sib2.intraFreqCellReselectionInfo.s_IntraSearchP = sib2conf.s_IntraSearchP;
    if (sib2conf.s_IntraSearchQ) {
        asn::MakeNew(sib2.intraFreqCellReselectionInfo.s_IntraSearchQ);
        *sib2.intraFreqCellReselectionInfo.s_IntraSearchQ = *sib2conf.s_IntraSearchQ;
    }
    sib2.intraFreqCellReselectionInfo.t_ReselectionNR = sib2conf.t_ReselectionNR;
    sib2.intraFreqCellReselectionInfo.deriveSSB_IndexFromCell = sib2conf.deriveSSB_IndexFromCell;

    asn::SequenceAdd(si_ies->sib_TypeAndInfo, ie_sib2);

    return pdu;
}

void GnbRrcTask::onBroadcastTimerExpired()
{
    triggerSysInfoBroadcast();
}

void GnbRrcTask::triggerSysInfoBroadcast()
{
    auto *mib = ConstructMibMessage(m_isBarred, m_config->intraFreqReselectionAllowed);
    auto *sib1 = ConstructSib1Message(m_cellReserved, m_config->tac, m_config->nci, m_config->plmn, m_aiBarringSet, m_config->sib1conf);
    auto *sib2 = ConstructSib2Message(m_config->sib2conf);

    sendRrcMessage(mib);
    sendRrcMessage(sib1);
    sendRrcMessage(sib2);

    asn::Free(asn_DEF_ASN_RRC_BCCH_BCH_Message, mib);
    asn::Free(asn_DEF_ASN_RRC_BCCH_DL_SCH_Message, sib1);
    asn::Free(asn_DEF_ASN_RRC_BCCH_DL_SCH_Message, sib2);
}

} // namespace nr::gnb