#A helper script for installation of the TIPsort codes

#check directory the script is being run from
if [ ! -d current ]; then
	echo "This script must be run from the TIPsort base directory."
	exit
fi

#check for properly set up ROOT5
echo "Checking ROOT5 installation..."
if [ -z "$ROOT5SYS" ]
then 
		echo "ROOT5SYS environment variable is not set!"
		echo "Please enter the path to the ROOT installation (base 'root' directory, which contains both 'lib' and 'include' directories): "
		read ROOT5SYS
		echo "" >> ~/.bashrc
		echo "#Set up ROOT5" >> ~/.bashrc
		echo "export ROOT5SYS=$ROOT5SYS" >> ~/.bashrc
else 
		echo "ROOT5SYS environment variable is set properly."
fi

if [ -z "$ROOT5INC" ]
then 
		echo "ROOT5INC environment variable is not set!  Setting..."
		echo "#Set up ROOT5INC" >> ~/.bashrc
		echo "export ROOT5INC=\$ROOT5SYS/include/root" >> ~/.bashrc
else 
		echo "ROOT5INC environment variable is set properly."
fi

if [ -z "$ROOT5LIB" ]
then 
		echo "ROOT5LIB environment variable is not set!  Setting..."
		echo "#Set up ROOT5LIB" >> ~/.bashrc
		echo "export ROOT5LIB=\$ROOT5SYS/lib/root" >> ~/.bashrc
else 
		echo "ROOT5LIB environment variable is set properly."
fi

source ~/.bashrc


echo "Setting up TIPsort directory structure..."
if [ -d bin ]; then
  rm -rf bin
fi
mkdir bin
mkdir bin/midas
mkdir bin/sfu

read -p "Ready to build TIPsort codes.  Press any key to start..."

echo "Making TIPsort codes..."
cd current
make

#return to the original directory
cd ..

#set up environment variables if neccesary
if [ -z "$TIPsort" ]
then
		echo "Setting up environment variables in .bashrc file..."
		tipsort_loc=$(pwd)
		echo "" >> ~/.bashrc
		echo "#Set up TIPsort" >> ~/.bashrc
		echo "export TIPsort="$tipsort_loc"" >> ~/.bashrc
		echo "export PATH=\$PATH:\$TIPsort/bin:\$TIPsort/bin/midas:\$TIPsort/bin/sfu" >> ~/.bashrc
		source ~/.bashrc
		echo "Environment variables set up!"
else
		echo "Environment variables are already set up."
fi

echo "TIPsort code installation is complete."
