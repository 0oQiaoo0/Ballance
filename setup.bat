echo copy bin to outdir
IF  NOT EXIST ..\Bin\_backup_vc16win64 (
        xcopy ..\Bin\vc16win64 ..\Bin\_backup_vc16win64 /E /Y /I	
)
xcopy Bin\vc16win64 ..\Bin\vc16win64 /E /Y /I
pause