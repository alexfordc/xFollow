@echo off

md vsVcxproj
cd vsVcxproj
md vs2012
md vs2017
cd ..

echo ��vs2017������ļ�������vs2017�б���

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


echo ��vs2012�е��ļ���������

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
