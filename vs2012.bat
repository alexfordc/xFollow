@echo off

md vsVcxproj
cd vsVcxproj
md vs2012
md vs2017
cd ..

echo 把vs2017的相关文件拷贝到vs2017中备份

copy xFollow.sln .\vsVcxproj\vs2017\ /Y

copy .\xCTPPlugin\xCTPPlugin.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\xCTPPlugin\xCTPPlugin.vcxproj.filters .\vsVcxproj\vs2017\ /Y

copy .\xFollow\xFollow.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\xFollow\xFollow.vcxproj.filters .\vsVcxproj\vs2017\ /Y

copy .\xFollowCenter\xFollowCenter.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\xFollowCenter\xFollowCenter.vcxproj.filters .\vsVcxproj\vs2017\ /Y

copy .\xFollowStrategy\xFollowStrategy.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\xFollowStrategy\xFollowStrategy.vcxproj.filters .\vsVcxproj\vs2017\ /Y

copy .\XNYSTools\XNYSTools.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\XNYSTools\XNYSTools.vcxproj.filters .\vsVcxproj\vs2017\ /Y

copy .\xTraderManage\xTraderManage.vcxproj .\vsVcxproj\vs2017\ /Y
copy .\xTraderManage\xTraderManage.vcxproj.filters .\vsVcxproj\vs2017\ /Y


echo 把vs2012中的文件拷贝出来

copy .\vsVcxproj\vs2012\xFollow.sln xFollow.sln /Y

copy .\vsVcxproj\vs2012\xCTPPlugin.vcxproj .\xCTPPlugin\ /Y
copy .\vsVcxproj\vs2012\xCTPPlugin.vcxproj.filters .\xCTPPlugin\ /Y
copy .\vsVcxproj\vs2012\xFollow.vcxproj .\xFollow\ /Y
copy .\vsVcxproj\vs2012\xFollow.vcxproj.filters .\xFollow\ /Y
copy .\vsVcxproj\vs2012\xFollowCenter.vcxproj .\xFollowCenter\ /Y
copy .\vsVcxproj\vs2012\xFollowCenter.vcxproj.filters .\xFollowCenter\ /Y
copy .\vsVcxproj\vs2012\xFollowStrategy.vcxproj .\xFollowStrategy\ /Y
copy .\vsVcxproj\vs2012\xFollowStrategy.vcxproj.filters .\xFollowStrategy\ /Y
copy .\vsVcxproj\vs2012\XNYSTools.vcxproj .\XNYSTools\ /Y
copy .\vsVcxproj\vs2012\XNYSTools.vcxproj.filters .\XNYSTools\ /Y
copy .\vsVcxproj\vs2012\xTraderManage.vcxproj .\xTraderManage\ /Y
copy .\vsVcxproj\vs2012\xTraderManage.vcxproj.filters .\xTraderManage\ /Y

pause
