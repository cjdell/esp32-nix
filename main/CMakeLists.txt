set(COMPONENT_REQUIRES "WiFiManager" "ESP32-audioI2S")

idf_component_register(SRCS "main_sin_task.cpp" # "main.cpp" "wifi.cpp" "fs.cpp" "audio.cpp"
                       INCLUDE_DIRS "."
                      )
