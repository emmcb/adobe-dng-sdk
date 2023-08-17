# This script copy MiniPDFL framework inside PDF_Handler resoures and also update it's install path 
# so that MiniPDFL does not need to be present in @executable_path/../Frameworks/ 

rm -rf ${CONFIGURATION_BUILD_DIR}/PDF_Handler.xpi/Resources/MiniPDFL.framework
cp -a ${PROJECT_DIR}/../../third-party/MiniPDFL/libraries/macintosh/${CT_RELEASE_DIR_TARGET}/release/MiniPDFL.framework ${CONFIGURATION_BUILD_DIR}/PDF_Handler.xpi/Resources/
install_name_tool -change @executable_path/../Frameworks/MiniPDFL.framework/Versions/A/MiniPDFL @loader_path/Resources/MiniPDFL.framework/Versions/A/MiniPDFL  ${CONFIGURATION_BUILD_DIR}/PDF_Handler.xpi/PDF_Handler
