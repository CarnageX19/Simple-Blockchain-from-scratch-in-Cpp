Program uses libqrencode(https://github.com/fukuchi/libqrencode/) to generate a QR code in the console (doesnt work in MinGW)
You will have to import the build libraries from github repository, to do so
type the following command:

git clone https://github.com/fukuchi/libqrencode.git

Then Enter the folder you just downloaded by:

cd libqrencode

Now you are ready to compile the library and tool. Type the following commands:

./configure
make
sudo make install
sudo ldconfig

This compiles and installs the library and header file to the appropriate directories by default, /usr/local/lib and /usr/local/include.
To compile the C++ code you have to use -lqrencode flag along with g++. Type the following command to compile the code:

g++ main.cpp -o blockchain -lqrencode

Now the application has been compiled and the binary is saved as “blockchain”. To start the application, type the following command and press enter:

./blockchain
