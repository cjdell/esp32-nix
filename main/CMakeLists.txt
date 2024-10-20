set(COMPONENT_REQUIRES "WiFiManager")

idf_component_register(SRCS "main.cpp" "wifi.cpp" "fs.cpp"
                       INCLUDE_DIRS "."
                      )
