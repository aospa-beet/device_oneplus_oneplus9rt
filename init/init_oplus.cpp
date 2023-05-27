/*
 * Copyright (C) 2022 The LineageOS Project
 * SPDX-License-Identifier: Apache-2.0
 */

#include <android-base/logging.h>
#include <android-base/properties.h>

#define _REALLY_INCLUDE_SYS__SYSTEM_PROPERTIES_H_
#include <string>
#include <sys/_system_properties.h>
#include <vector>

using android::base::GetProperty;

/*
 * SetProperty does not allow updating read only properties and as a result
 * does not work for our use case. Write "OverrideProperty" to do practically
 * the same thing as "SetProperty" without this restriction.
 */
void OverrideProperty(const char *name, const char *value) {
  size_t valuelen = strlen(value);

  prop_info *pi = (prop_info *)__system_property_find(name);
  if (pi != nullptr) {
    __system_property_update(pi, value, valuelen);
  } else {
    __system_property_add(name, strlen(name), value, valuelen);
  }
}

void property_override(std::string prop, std::string value, bool add = true) {
  auto pi = (prop_info *)__system_property_find(prop.c_str());
  if (pi != nullptr) {
    __system_property_update(pi, value.c_str(), value.length());
  } else if (add) {
    __system_property_add(prop.c_str(), prop.length(), value.c_str(),
                          value.length());
  }
}

std::vector<std::string> ro_props_default_source_order = {
    "bootimage.",  "odm.",    "product.",     "system.",
    "system_ext.", "vendor.", "vendor_dlkm.", "",
};

/*
 * Only for read-only properties. Properties that can be wrote to more
 * than once should be set in a typical init script (e.g. init.oplus.hw.rc)
 * after the original property has been set.
 */
void vendor_load_properties() {
  auto rf_version = std::stoi(GetProperty("ro.boot.rf_version", "0"));
  auto prjname = std::stoi(GetProperty("ro.boot.prjname", "0"));
  std::string brand, device, manufacturer, model, name, fingerprint;

  switch (prjname) {
  case 20820: // CN
    OverrideProperty("ro.product.product.model", "MT2110");
    OverrideProperty("ro.product.product.device", "OP5154L1");
    break;
  case 20821: // IN
    OverrideProperty("ro.product.product.model", "MT2111");
    break;
  default:
    LOG(ERROR) << "Unexpected project name: " << prjname;
  }
  switch (rf_version) {
  case 2:
    for (const auto &source : ro_props_default_source_order) {
      brand = "ro.product." + source + "brand";
      device = "ro.product." + source + "device";
      manufacturer = "ro.product." + source + "manufacturer";
      model = "ro.product." + source + "model";
      name = "ro.product." + source + "name";
      fingerprint = "ro." + source + "build.fingerprint";

      property_override(brand, "realme", true);
      property_override(device, "RE58B2L1", true);
      property_override(manufacturer, "realme", true);
      property_override(model, "RMX3312", true);
      property_override(name, "RE58B2L1", true);
      property_override(fingerprint,
                        "realme/RMX3312/RE58B2L1:13/TP1A.220905.001/"
                        "S.d7a144-1-3cc75:user/release-keys",
                        true);
      OverrideProperty(
          "ro.build.description",
          "qssi-user 13 TP1A.220905.001 1673407054945 release-keys");
      OverrideProperty("ro.build.product", "RE58B2L1");
      OverrideProperty("ro.com.google.clientidbase", "android-oppo");
    }
    break;
  default:
    LOG(ERROR) << "Unexpected RF version: " << rf_version;
  }
}
