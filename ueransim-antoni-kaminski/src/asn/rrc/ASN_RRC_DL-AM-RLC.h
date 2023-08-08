/*
 * Generated by asn1c-0.9.29 (http://lionet.info/asn1c)
 * From ASN.1 module "NR-RRC-Definitions"
 * 	found in "asn/nr-rrc-15.6.0.asn1"
 * 	`asn1c -fcompound-names -pdu=all -findirect-choice -fno-include-deps -gen-PER -no-gen-OER -no-gen-example -D rrc`
 */

#ifndef	_ASN_RRC_DL_AM_RLC_H_
#define	_ASN_RRC_DL_AM_RLC_H_


#include <asn_application.h>

/* Including external dependencies */
#include "ASN_RRC_SN-FieldLengthAM.h"
#include "ASN_RRC_T-Reassembly.h"
#include "ASN_RRC_T-StatusProhibit.h"
#include <constr_SEQUENCE.h>

#ifdef __cplusplus
extern "C" {
#endif

/* ASN_RRC_DL-AM-RLC */
typedef struct ASN_RRC_DL_AM_RLC {
	ASN_RRC_SN_FieldLengthAM_t	*sn_FieldLength;	/* OPTIONAL */
	ASN_RRC_T_Reassembly_t	 t_Reassembly;
	ASN_RRC_T_StatusProhibit_t	 t_StatusProhibit;
	
	/* Context for parsing across buffer boundaries */
	asn_struct_ctx_t _asn_ctx;
} ASN_RRC_DL_AM_RLC_t;

/* Implementation */
extern asn_TYPE_descriptor_t asn_DEF_ASN_RRC_DL_AM_RLC;
extern asn_SEQUENCE_specifics_t asn_SPC_ASN_RRC_DL_AM_RLC_specs_1;
extern asn_TYPE_member_t asn_MBR_ASN_RRC_DL_AM_RLC_1[3];

#ifdef __cplusplus
}
#endif

#endif	/* _ASN_RRC_DL_AM_RLC_H_ */
#include <asn_internal.h>
