export PATH=/home/xieshuai/.Qt/QtIFW-3.0.6/bin:$PATH
echo "Update online repository>>>>>>>>>>>>>>>>>>>>>>>>>>"
repogen -v --update-new-components -p packages ../../repo/linux64
echo "Create offline installer>>>>>>>>>>>>>>>>>>>>>>>>>>"
binarycreator -v --offline-only -c config/config_lnx64.xml -p packages ../../offline/BjutNetLogin_3.1.1_Linux64.run
echo "Done>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>"
