//
// This file is a part of UERANSIM open source project.
// Copyright (c) 2021 ALİ GÜNGÖR.
//
// The software and all associated files are licensed under GPL-3.0
// and subject to the terms and conditions defined in LICENSE file.
//

#include "task.hpp"

#include <lib/asn/rrc.hpp>
#include <lib/asn/utils.hpp>
#include <lib/rrc/encode.hpp>
#include <ue/nas/task.hpp>

#include <asn/rrc/ASN_RRC_MIB.h>
#include <asn/rrc/ASN_RRC_PLMN-IdentityInfo.h>
#include <asn/rrc/ASN_RRC_PLMN-IdentityInfoList.h>
#include <asn/rrc/ASN_RRC_SIB1.h>
#include <asn/rrc/ASN_RRC_UAC-BarringInfoSet.h>
#include <asn/rrc/ASN_RRC_SIB2.h>

namespace nr::ue
{

void UeRrcTask::receiveMib(int cellId, const ASN_RRC_MIB &msg)
{
    auto &desc = m_cellDesc[cellId];

    desc.mib.isBarred = msg.cellBarred == ASN_RRC_MIB__cellBarred_barred;
    desc.mib.isIntraFreqReselectAllowed = msg.intraFreqReselection == ASN_RRC_MIB__intraFreqReselection_allowed;

    desc.mib.hasMib = true;

    updateAvailablePlmns();
}

void UeRrcTask::receiveSib1(int cellId, const ASN_RRC_SIB1 &msg)
{
    auto &desc = m_cellDesc[cellId];

    desc.sib1.sib1conf.q_RxLevMin = msg.cellSelectionInfo->q_RxLevMin;
    desc.sib1.sib1conf.q_RxLevMinOffset = *msg.cellSelectionInfo->q_RxLevMinOffset;
    desc.sib1.sib1conf.q_RxLevMinSUL = *msg.cellSelectionInfo->q_RxLevMinSUL;
    desc.sib1.sib1conf.q_QualMin = *msg.cellSelectionInfo->q_QualMin;
    desc.sib1.sib1conf.q_QualMinOffset = *msg.cellSelectionInfo->q_QualMinOffset;

    desc.sib1.isReserved = msg.cellAccessRelatedInfo.cellReservedForOtherUse != nullptr;

    auto *plmnIdentityInfo = msg.cellAccessRelatedInfo.plmn_IdentityList.list.array[0];
    desc.sib1.nci = asn::GetBitStringLong<36>(plmnIdentityInfo->cellIdentity);

    desc.sib1.isReserved &=
        plmnIdentityInfo->cellReservedForOperatorUse == ASN_RRC_PLMN_IdentityInfo__cellReservedForOperatorUse_reserved;

    desc.sib1.tac = asn::GetBitStringInt<24>(*plmnIdentityInfo->trackingAreaCode);

    auto plmnIdentity = plmnIdentityInfo->plmn_IdentityList.list.array[0];
    desc.sib1.plmn = asn::rrc::GetPlmnId(*plmnIdentity);

    auto *barringInfo = msg.uac_BarringInfo->uac_BarringInfoSetList.list.array[0];

    int barringBits = asn::GetBitStringInt<7>(barringInfo->uac_BarringForAccessIdentity);
    desc.sib1.aiBarringSet.ai15 = bits::BitAt<0>(barringBits);
    desc.sib1.aiBarringSet.ai14 = bits::BitAt<1>(barringBits);
    desc.sib1.aiBarringSet.ai13 = bits::BitAt<2>(barringBits);
    desc.sib1.aiBarringSet.ai12 = bits::BitAt<3>(barringBits);
    desc.sib1.aiBarringSet.ai11 = bits::BitAt<4>(barringBits);
    desc.sib1.aiBarringSet.ai2 = bits::BitAt<5>(barringBits);
    desc.sib1.aiBarringSet.ai1 = bits::BitAt<6>(barringBits);

    desc.sib1.hasSib1 = true;

    updateAvailablePlmns();
}

void UeRrcTask::receiveSib2(int cellId, const ASN_RRC_SIB2 &msg) {
    auto &desc = m_cellDesc[cellId];

    desc.sib2.sib2conf.q_Hyst = msg.cellReselectionInfoCommon.q_Hyst;
    desc.sib2.sib2conf.treshServingLowP = msg.cellReselectionServingFreqInfo.threshServingLowP;
    desc.sib2.sib2conf.cellReselectionPriority = msg.cellReselectionServingFreqInfo.cellReselectionPriority;
    desc.sib2.sib2conf.q_RxLevMin = msg.intraFreqCellReselectionInfo.q_RxLevMin;
    desc.sib2.sib2conf.s_IntraSearchP = msg.intraFreqCellReselectionInfo.s_IntraSearchP;
    desc.sib2.sib2conf.s_IntraSearchQ = *msg.intraFreqCellReselectionInfo.s_IntraSearchQ;
    desc.sib2.sib2conf.t_ReselectionNR = msg.intraFreqCellReselectionInfo.t_ReselectionNR;
    desc.sib2.sib2conf.deriveSSB_IndexFromCell = msg.intraFreqCellReselectionInfo.deriveSSB_IndexFromCell;

    desc.sib2.hasSib2 = true;

    updateAvailablePlmns();
}

} // namespace nr::ue