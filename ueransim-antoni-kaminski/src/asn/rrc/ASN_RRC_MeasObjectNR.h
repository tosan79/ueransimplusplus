/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_MeasObjectNR_H_
#define	_ASN_RRC_MeasObjectNR_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ASN_RRC_ARFCN-ValueNR.h"
#include "ASN_RRC_SubcarrierSpacing.h"
#include "ASN_RRC_ReferenceSignalConfig.h"
#include <NativeInteger.h>
#include "ASN_RRC_Q-OffsetRangeList.h"
#include <asn_SEQUENCE_OF.h>
#include <constr_SEQUENCE_OF.h>
#include "ASN_RRC_FreqBandIndicatorNR.h"
#include <NativeEnumerated.h>
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Dependencies */
typedef enum ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530 {
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf160	= 0,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf256	= 1,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf320	= 2,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf512	= 3,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf640	= 4,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf1024	= 5,
	ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530_sf1280	= 6
} e_ASN_RRC_MeasObjectNR__ext1__measCycleSCell_v1530;

/* Forward declarations */
struct ASN_RRC_SSB_MTC;
struct ASN_RRC_SSB_MTC2;
struct ASN_RRC_ThresholdNR;
struct ASN_RRC_PCI_List;
struct ASN_RRC_CellsToAddModList;
struct ASN_RRC_PCI_RangeIndexList;
struct ASN_RRC_PCI_RangeElement;

/* ASN_RRC_MeasObjectNR */
typedef struct ASN_RRC_MeasObjectNR {
	ASN_RRC_ARFCN_ValueNR_t	*ssbFrequency;	/* OPTIONAL */
	ASN_RRC_SubcarrierSpacing_t	*ssbSubcarrierSpacing;	/* OPTIONAL */
	struct ASN_RRC_SSB_MTC	*smtc1;	/* OPTIONAL */
	struct ASN_RRC_SSB_MTC2	*smtc2;	/* OPTIONAL */
	ASN_RRC_ARFCN_ValueNR_t	*refFreqCSI_RS;	/* OPTIONAL */
	ASN_RRC_ReferenceSignalConfig_t	 referenceSignalConfig;
	struct ASN_RRC_ThresholdNR	*absThreshSS_BlocksConsolidation;	/* OPTIONAL */
	struct ASN_RRC_ThresholdNR	*absThreshCSI_RS_Consolidation;	/* OPTIONAL */
	long	*nrofSS_BlocksToAverage;	/* OPTIONAL */
	long	*nrofCSI_RS_ResourcesToAverage;	/* OPTIONAL */
	long	 quantityConfigIndex;
	ASN_RRC_Q_OffsetRangeList_t	 offsetMO;
	struct ASN_RRC_PCI_List	*cellsToRemoveList;	/* OPTIONAL */
	struct ASN_RRC_CellsToAddModList	*cellsToAddModList;	/* OPTIONAL */
	struct ASN_RRC_PCI_RangeIndexList	*blackCellsToRemoveList;	/* OPTIONAL */
	struct ASN_RRC_MeasObjectNR__blackCellsToAddModList {
		A_SEQUENCE_OF(struct ASN_RRC_PCI_RangeElement) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *blackCellsToAddModList;
	struct ASN_RRC_PCI_RangeIndexList	*whiteCellsToRemoveList;	/* OPTIONAL */
	struct ASN_RRC_MeasObjectNR__whiteCellsToAddModList {
		A_SEQUENCE_OF(struct ASN_RRC_PCI_RangeElement) list;
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *whiteCellsToAddModList;
	/*
	 * This type is extensible,
	 * possible extensions are below.
	 */
	struct ASN_RRC_MeasObjectNR__ext1 {
		ASN_RRC_FreqBandIndicatorNR_t	*freqBandIndicatorNR_v1530;	/* OPTIONAL */
		long	*measCycleSCell_v1530;	/* OPTIONAL */
		
		/* Context for parsing across buffer boundaries */
		asn_struct_ctx_t _asn_ctx;
	} *ext1;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ASN_RRC_MeasObjectNR_t;

/* Implementation */
/* extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_measCycleSCell_v1530_25;	// (Use -fall-defs-global to expose) */
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_MeasObjectNR;
extern asn_SEQUENCE_specifics_t asn_SPC_ASN_RRC_MeasObjectNR_specs_1;
extern asn_TYPE_member_t asn_MBR_ASN_RRC_MeasObjectNR_1[19];

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_MeasObjectNR_H_ */
#include <asn_internal.h>
