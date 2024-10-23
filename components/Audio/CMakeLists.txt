cmake_minimum_required(VERSION 3.5)

idf_component_register(
                       SRCS "I2SBase.cpp" "PDMOutput.cpp" "PDMTimerOutput.cpp" "WAVFileReader.cpp"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES arduino-esp32
)

project(Audio)
