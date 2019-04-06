# OpenCLError-Test
In termux
Initially  you have to make sure that you have the libOpenCL.so file in /system/vendor/lib64 folder.
Then exectue following command in github folder
g++ -L /system/vendor/lib64 -w  -o test  *.cpp -lOpenCL -I ./; ./test 
