
const int nBins=200;

// https://indico.cern.ch/event/1316875/contributions/5540132/attachments/2699465/4687614/2022centralityCalibration_v2.pdf
const Double_t binTable[nBins+1] = {0, 11.94, 12.7858, 13.6009, 14.4157, 15.2418, 16.1033, 16.9536, 17.8246, 18.7434, 19.6682, 20.6067, 21.5663, 22.5606,
                                    23.5999, 24.7189, 25.8429, 27.0055, 28.2138, 29.4384, 30.7344, 32.0919, 33.4997, 34.9442, 36.4639, 38.0261, 39.672,
                                    41.4021, 43.2345, 45.1833, 47.2025, 49.2579, 51.3781, 53.6633, 55.9937, 58.4434, 61.0549, 63.813, 66.6208, 69.574,
                                    72.6381, 75.7773, 79.0623, 82.501, 86.0189, 89.6624, 93.4657, 97.3644, 101.413, 105.489, 110.003, 114.082, 118.738,
                                    123.365, 128.425, 133.482, 138.917, 144.363, 149.93, 155.55, 162.165, 168.71, 175.473, 182.173, 188.771,
                                    195.858,203.179, 210.514, 218.562, 225.911, 234.745, 243.603, 252.338, 261.092, 270.109, 280.398, 290.593, 301.152,
                                    311.505, 322.096, 333.815, 345.082, 356.813, 368.83, 381.476, 394.571, 407.889, 420.584, 434.255, 448.845, 462.808,
                                    478.615, 493.181, 508.257, 525.151, 541.433, 558.101, 576.411, 593.964, 611.396, 629.212, 646.977, 665.27, 685.814,
                                    705.177, 725.066, 746.338, 766.521, 788.188, 810.009, 832.129, 854.86, 876.852, 901.551, 923.876, 949.075, 974.073,
                                    999.683, 1026.65, 1051.24, 1079.2, 1105.73, 1134.35, 1163.15, 1192.15, 1221.5, 1251.06, 1282.89, 1313.56, 1344.4,
                                    1377.29,1408.65, 1443.22, 1476.76, 1510.13, 1547.41, 1582.48, 1620.62, 1658.59, 1694.91, 1730.72, 1772.39, 1812.57,
                                    1852.17, 1892.2, 1931.65, 1972.9, 2014.34, 2059.92, 2102.12, 2145.72, 2189.71, 2236.86, 2281.99, 2330, 2380.49,
                                    2430.08, 2480.69, 2531.56, 2585.02, 2635.88, 2691.91, 2746.83, 2801.4, 2859.98, 2918.62, 2976.77, 3036.56, 3101.15,
                                    3162.96, 3224.24, 3285.01, 3351.95, 3417.69, 3490.35, 3556.81, 3633.27, 3706.18, 3781.61, 3863.89, 3943.01, 4026.55,
                                    4112.04, 4198.95, 4290.06, 4387.63, 4487.31, 4585.16, 4692.11, 4799.22, 4913.78, 5035.08, 5148.93, 5274.96, 5405.71,
                                    5545.51,5699.7, 5866.89, 6052.83, 6252.21, 2.e+4};

Int_t getHiBinFromhiHF(const Double_t hiHF)
{
  Int_t binPos = -1;
  for(int i = 0; i < nBins; ++i){
    if(hiHF >= binTable[i] && hiHF < binTable[i+1]){
      binPos = i;
      break; 
    }
  }                   

  binPos = nBins - 1 - binPos;

  return (Int_t)(200*((Double_t)binPos)/((Double_t)nBins));
}
