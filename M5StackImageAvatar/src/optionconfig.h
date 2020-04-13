// Copyright(c) 2020 Takao Akaki
#ifndef _OPTIONCONFIG_H_
#define _OPTIONCONFIG_H_ 

#ifdef USE_WIFI
  // WiFi Settings
  const char* ssid = ""; // your ssid 
  const char* pass = ""; // your ssid's passphrase
#endif

#ifdef USE_TTS
  // AquesTalk Data
  #define MAX_SERIF 2
  const char* serif[MAX_SERIF] = {
    "emufaibu,sutakku,tanoshiiyo",
    "watashiwa,gotsumi,homura,desu",
  };
#endif

#endif // _OPTIONCONFIG_H_
