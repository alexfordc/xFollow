@echo off

md vsVcxproj
cd vsVcxproj
md vs2012
md vs2017
cd ..

echo 把vs2012的相关文件拷贝到vs2012中备份

copy xFollow.sln .\vsVcxproj\vs2012\ /Y

copy .\xCTPPlugin\xCTPPlugin.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\xCTPPlugin\xCTPPlugin.vcxproj.filters .\vsVcxproj\vs2012\ /Y

copy .\xFollow\xFollow.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\xFollow\xFollow.vcxproj.filters .\vsVcxproj\vs2012\ /Y

copy .\xFollowCenter\xFollowCenter.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\xFollowCenter\xFollowCenter.vcxproj.filters .\vsVcxproj\vs2012\ /Y

copy .\xFollowStrategy\xFollowStrategy.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\xFollowStrategy\xFollowStrategy.vcxproj.filters .\vsVcxproj\vs2012\ /Y

copy .\XNYSTools\XNYSTools.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\XNYSTools\XNYSTools.vcxproj.filters .\vsVcxproj\vs2012\ /Y

copy .\xTraderManage\xTraderManage.vcxproj .\vsVcxproj\vs2012\ /Y
copy .\xTraderManage\xTraderManage.vcxproj.filters .\vsVcxproj\vs2012\ /Y


echo 把vs2017中的文件拷贝出来

copy .\vsVcxproj\vs2017\xFollow.sln xFollow.sln /Y

copy .\vsVcxproj\vs2017\xCTPPlugin.vcxproj .\xCTPPlugin\ /Y
copy .\vsVcxproj\vs2017\xCTPPlugin.vcxproj.filters .\xCTPPlugin\ /Y
copy .\vsVcxproj\vs2017\xFollow.vcxproj .\xFollow\ /Y
copy .\vsVcxproj\vs2017\xFollow.vcxproj.filters .\xFollow\ /Y
copy .\vsVcxproj\vs2017\xFollowCenter.vcxproj .\xFollowCenter\ /Y
copy .\vsVcxproj\vs2017\xFollowCenter.vcxproj.filters .\xFollowCenter\ /Y
copy .\vsVcxproj\vs2017\xFollowStrategy.vcxproj .\xFollowStrategy\ /Y
copy .\vsVcxproj\vs2017\xFollowStrategy.vcxproj.filters .\xFollowStrategy\ /Y
copy .\vsVcxproj\vs2017\XNYSTools.vcxproj .\XNYSTools\ /Y
copy .\vsVcxproj\vs2017\XNYSTools.vcxproj.filters .\XNYSTools\ /Y
copy .\vsVcxproj\vs2017\xTraderManage.vcxproj .\xTraderManage\ /Y
copy .\vsVcxproj\vs2017\xTraderManage.vcxproj.filters .\xTraderManage\ /Y

pause
