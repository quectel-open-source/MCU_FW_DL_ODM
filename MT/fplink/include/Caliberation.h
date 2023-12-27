/* Copyright (C) 2015 RDA Technologies Limited and/or its affiliates("RDA").
 * All rights reserved.
 *
 * This software is supplied "AS IS" without any warranties.
 * RDA assumes no responsibility or liability for the use of the software,
 * conveys no license or title under any patent, copyright, or mask work
 * right to the product. RDA reserves the right to make changes in the
 * software without notification.  RDA also make no representation or
 * warranty that such application will be suitable for the specified use
 * without further testing or modification.
 */

#ifndef __CALIBERATION_H_INCLUDED__
#define __CALIBERATION_H_INCLUDED__

#include "fplink_export.h"
#include <string>
#include <list>
#include <stdint.h>

class TargetConnection;
class ChipInfo;
class FlashInfo;
class FlashProgram;
class LodData;

class FPLINK_API Caliberation
{
public:
    Caliberation() {}
    virtual ~Caliberation() {}

    // It is not really caliberation feature. However, the caliberation
    // data structure is too complex.
    virtual int burnAudioData(FlashProgram* programmer, const FlashInfo& flashInfo, void* data) = 0;
    virtual int burnRFData(FlashProgram* programmer, const FlashInfo& falshInfo, const std::list<LodData*> &lods) = 0;
    virtual int checkRfCrc(FlashProgram* programmer, const FlashInfo& flashInfo) = 0;
    virtual int checkRfCrc(const FlashInfo& flashInfo) = 0;
    // Get the 32bits RF caliberation flag. (1<<31) is calib ok.
    enum { CALIB_FLAG = 31,
           CALIB_GSM850_AFC = 2,
           CALIB_EGSM_AFC = 3,
           CALIB_DCS_AFC = 4,
           CALIB_PCS_AFC = 5,
           CALIB_GSM850_APC = 10,
           CALIB_EGSM_APC = 11,
           CALIB_DCS_APC = 12,
           CALIB_PCS_APC = 13,
           CALIB_GSM850_AGC = 14,
           CALIB_EGSM_AGC = 15,
           CALIB_DCS_AGC = 16,
           CALIB_PCS_AGC = 17
         };

    enum { NB_CALIB_FLAG = 15,
           NB_CALIB_CRYSTAL = 0,
           NB_CALIB_IBAND1_APC = 1,
           NB_CALIB_IBAND3_APC = 2,
           NB_CALIB_IBAND5_APC = 3,
           NB_CALIB_IBAND8_APC = 4,
           NB_CALIB_IBAND20_APC = 5,
           NB_CALIB_IBAND28_APC = 6,
           NB_CALIB_IBAND1_AGC = 7,
           NB_CALIB_IBAND3_AGC = 8,
           NB_CALIB_IBAND5_AGC = 9,
           NB_CALIB_IBAND8_AGC = 10,
           NB_CALIB_IBAND20_AGC = 11,
           NB_CALIB_IBAND28_AGC = 12,
         };
    virtual int getRfCalibFlag(const FlashInfo& flashInfo, uint32_t& flag) = 0;

    virtual int read32(uint32_t targetAddress, uint32_t& val) = 0;

    virtual int prepare(std::string& xcvName, std::string& paName, std::string& swName) = 0;
    virtual int init(bool& calibNotMod, bool& phyNotReady) = 0;
    virtual int get_versions(int& major, int& minor) = 0;
    virtual int send_cmd(uint32_t cmd) = 0;
    virtual int update() = 0;
    virtual int send_mode(uint32_t mode) = 0;
    virtual int set_method(bool automaticTool) = 0;
    virtual int mode_rx(uint8_t band, uint16_t arfcn, uint8_t expPow, uint32_t tag) = 0;
    virtual int mode_tx(uint8_t band, uint16_t arfcn, uint8_t pcl, uint16_t dacIdVal, uint8_t tsc, uint32_t tag) = 0;
    // int get_versions();
    virtual int get_rf_chip_names(uint32_t& xcvName, uint32_t& paName, uint32_t& swName) = 0;
    virtual int set_xcv_time(int id, int16_t time) = 0;
    virtual int set_pa_time(int id, int16_t time) = 0;
    virtual int set_sw_time(int id, int16_t time) = 0;
    virtual int set_testflag(int id, uint32_t flag) {
        return -1;
    }
    virtual int set_updatetestflag(uint32_t flag) {
        return -1;
    }
    virtual int set_pal_time(int id, int16_t time) = 0;
    virtual int set_xcv_param(int id, int32_t param) = 0;
    virtual int set_pa_param(int id, int32_t param) = 0;
    virtual int set_sw_param(int id, int32_t param) = 0;
    virtual int set_pa_profile_interp(int band, int id, uint16_t val) = 0;
    virtual int set_pa_profile_interps(int band, uint16_t val[]) = 0;
    virtual int set_rxtx_freq_offset(int band, int16_t fof) = 0;
    virtual int set_rxtx_time_offset(int16_t tof) = 0;
    virtual int set_target_power_per_pcl(int band, int pclId, int16_t power) = 0;
    virtual int set_power_per_pcl_per_arfcn(int band, int pclId, int arfcn, int16_t measuredPower) = 0;
    virtual int set_powers_per_pcl_per_arfcn(int band, int16_t powbuffer0[], int16_t powbuffer1[]) = 0;
    virtual int set_default_power_per_pcl_per_arfcn(int band) = 0;
    virtual int set_default_pcl() = 0;
    virtual int set_profile(int band, int16_t measuredPowers[]) {
        return -1;
    }
    virtual int set_ramp_low(int id, uint16_t val) = 0;
    virtual int set_ramp_high(int id, uint16_t val) = 0;
    virtual int set_ramp_curve(int curvenum, int id, uint8_t val) {
        return -1;
    }
    virtual int set_ramp_switch(int band, uint16_t val) = 0;
    virtual int set_low_volt(int id, uint16_t val) = 0;
    virtual int set_low_dac(int id, uint16_t val) = 0;
    // int set_voc_ec();
    // int set_voc_filters();
    // int set_gains();
    virtual int set_gpadc_bandgap(uint8_t bandgap) = 0;
    virtual int set_gpadc_sensor(uint16_t gainA, uint16_t gainB) = 0;
    // int set_rf_analog();
    virtual int set_rx_iq_time_offset(int16_t offset) = 0;
    virtual int set_tx_iq_time_offset(int16_t offset) = 0;

    virtual int get_xcv_time(int id, int16_t& time) = 0;
    virtual int get_pa_time(int id, int16_t& time) = 0;
    virtual int get_sw_time(int id, int16_t& time) = 0;
    virtual int get_pal_time(int id, int16_t& time) = 0;
    virtual int get_xcv_param(int id, int32_t& param) = 0;
    virtual int get_pa_param(int id, int32_t& param) = 0;
    virtual int get_sw_param(int id, int32_t& param) = 0;
    virtual int get_pa_profile_interp(int band, int id, uint16_t& val) = 0;
    virtual int get_rxtx_freq_offset(int band, int16_t& val) = 0;
    virtual int get_rxtx_time_offset(int16_t& val) = 0;
    virtual int get_target_power_per_pcl(int band, int pclId, int16_t& val) = 0;
    virtual int get_power_per_pcl_per_arfcn(int band, int pclId, int arfcn, uint16_t& val) = 0;
    virtual int get_profile(int band, int id, uint16_t& val) = 0;
    virtual int get_profile_extrems(int band, int16_t& valMin, int16_t& valMax) = 0;
    virtual int get_ramp_low(int id, uint16_t& val) = 0;
    virtual int get_ramp_high(int id, uint16_t& val) = 0;
    virtual int get_ramp_curve(int curvenum, int id, uint8_t& val) {
        return -1;
    }
    virtual int get_ramp_switch(int band, uint16_t& val) = 0;
    virtual int get_low_volt(int id, uint16_t& val) = 0;
    virtual int get_low_dac(int id, uint16_t& val) = 0;
    // int get_voc_ec();
    // int get_voc_filters();
    // int get_gains();
    // int get_gpadc_bandgap();
    virtual int get_gpadc_sensor(uint16_t& gainA, uint16_t& gainB) = 0;
    // int get_rf_analog();
    virtual int get_rx_iq_time_offset(int16_t& val) = 0;
    virtual int get_tx_iq_time_offset(int16_t& val) = 0;

    virtual int mode_stop() = 0;
    virtual int mode_rx_freq(uint8_t band, uint16_t arfcn, uint8_t expPow) = 0;
    virtual int mode_rx_power(uint8_t band, uint16_t arfcn, uint8_t expPow) = 0;
    virtual int mode_rx_stat(uint8_t band, uint16_t arfcn, uint8_t expPow) = 0;
    virtual int mode_tx_pcl(uint8_t band, uint16_t arfcn, uint8_t pcl, uint8_t tsc) = 0;
    virtual int mode_tx_pa_idx(uint8_t band, uint16_t arfcn, uint16_t dacIdVal, uint8_t tsc) = 0;
    virtual int mode_tx_pa_val(uint8_t band, uint16_t arfcn, uint16_t dacIdVal, uint8_t tsc) = 0;
    // int mode_audio_out();
    virtual int set_xtal_freq_offset(int32_t xtalFreqOffset, bool& stop) = 0;
    virtual int set_afc_bound(uint8_t afcBound) = 0;
    virtual int set_changedacafc_bound(uint8_t afcBound) = 0;
    virtual int set_changedacafc_freq_offset(int32_t afcFreqOffset) = 0;
    virtual int set_changedacafc_midfreq_offset(int32_t afcFreqOffset) = 0;
    virtual int set_afc_freq_offset(int32_t afcFreqOffset) = 0;
    virtual int set_iloss_offset(int8_t ilossOffset, uint16_t& iLossNextArfcn, bool& stop) = 0;
    virtual int set_pa_profile_meas(uint8_t band, int32_t measPower, uint16_t& paProfNextDacVal, uint8_t& paProfCalibDone) = 0;
    virtual int set_pa_profile_meass(uint8_t band, int32_t measPower[], uint16_t paDacVal[]) = 0;
    virtual int set_pmd_custom(uint8_t customCommand, uint32_t& pmdCustomCalibStatus) = 0;
    virtual int set_update_values() = 0;
    // int set_dictaphone_record();
    // int set_dictaphone_playback();
    // int get_rx_fof();
    // int get_rx_mon_power();
    virtual int get_rx_nb_power(uint8_t& val) = 0;
    // int get_rx_stat();
    virtual int get_gpadc_value_ch(int ch, uint16_t& measure) = 0;
    virtual int get_gpadc_values(uint16_t measures[4]) = 0;
    // int get_iq_dc_offsets();
    // int reset_parameters();
    virtual int reset_rfparameters() {
        return -1;
    }
    virtual int reset_bbparameters() {
        return -1;
    }
    // int reset_rf_parameters();
    // int reset_bb_parameters();
    // int erase_flash();
    virtual int burn_flash(bool automaticTool) = 0;
    virtual int get_calibflag(uint32_t& flag) = 0;
    virtual int dump_parameters(char * fname, bool automaticTool) = 0;
    virtual int dump_parameters(LodData *lod, bool automaticTool) = 0;
    virtual int autoCallTest(uint32_t cmd, int& result) = 0;
    virtual int set_Nsft_configure_req(uint8_t& rspid, uint8_t& status) = 0;
    virtual int set_Nsft_berloop_ab_orderreq(int rfset, uint8_t& rspid, uint8_t& status) = 0;
    virtual int set_Nsft_ber_start_log_req(int rfset, uint8_t& rspid, uint8_t& status) = 0;
    virtual int set_Nsft_ber_stop_log_req(uint8_t& rspid, uint8_t& status) = 0;
    virtual int get_NsftComstatus(uint8_t& rspid, uint8_t& status) = 0;
    virtual int get_NsftSyncBer(uint8_t& syncber) = 0;
    virtual int get_NsftBitNumber(uint32_t& nbBits, uint32_t& nbErr, uint32_t& logindex) = 0;
    virtual int set_connectNs(int rfset, uint8_t band, uint16_t tch, uint16_t bch, uint8_t pcl, uint8_t& rspid, uint8_t& status) = 0;
    virtual int start_stub() = 0;
    virtual int gps_start() = 0;
    virtual int shut_down() = 0;

    //nbiot
    virtual int nbiot_mode_tx_power(uint8_t band, uint16_t arfcn, int16_t power,uint8_t friststate)=0;
    virtual int nbiot_mode_tx(uint8_t band, uint16_t arfcn, int16_t power, uint32_t tag,uint8_t friststate)=0;
    virtual int nbiot_mode_rx_power(uint8_t band, uint16_t arfcn, int16_t expPow,uint8_t friststate)=0;
    virtual int nbiot_mode_rx(uint8_t band, uint16_t arfcn, int16_t expPow, uint32_t tag,uint8_t friststate)=0;
    virtual int nbiot_set_iloss_offset(int8_t ilossOffset,uint8_t band, uint16_t& iLossNextArfcn, bool& stop)=0;
    virtual int nbiot_get_rx_nb_power(uint16_t &val)=0;
    virtual int nbiot_send_mode(uint32_t mode,uint8_t friststate)=0;
    virtual int nbiot_set_rx_nb_power(uint16_t &val)=0;
    virtual int nbiot_set_tx_poweroffset(uint8_t band,int16_t &measpower,uint16_t &nextarfcn,int16_t MeasTxPower,bool&stop)=0;
    virtual int nbiot_set_connectNs(uint8_t band, uint16_t arfcn, int16_t txpower, int16_t rxpower)=0;
    virtual int nbiot_get_NsftComstatus(uint16_t& crnti, uint32_t& status)=0;
	virtual int Nbiot_NsftRxMeasure(float* measber,uint8_t band, uint16_t arfcn,int16_t rxpower)=0;
	virtual int Nbiot_NsftTxMeasure(uint8_t band, uint16_t arfcn, int16_t power)=0;

};

// Get caliberation version from map table, and create caliberation based on version.
// DON'T call this when FLASH is empty or RAMRUN is loaded.
FPLINK_API Caliberation* CalibCreateByMap(TargetConnection* conn, ChipInfo* chip);

// Get caliberation version from FLASH.
// Call this only valid FLASH caliberation data is required.
FPLINK_API Caliberation* CalibCreateByFlash(TargetConnection* conn, ChipInfo* chip, uint32_t calibRfBase);

FPLINK_API Caliberation* Calib26Create(TargetConnection* conn, ChipInfo* chip);
FPLINK_API Caliberation* Calib27Create(TargetConnection* conn, ChipInfo* chip);
FPLINK_API Caliberation* Calib28Create(TargetConnection* conn, ChipInfo* chip);
FPLINK_API Caliberation* Calib29Create(TargetConnection* conn, ChipInfo* chip);

#endif
