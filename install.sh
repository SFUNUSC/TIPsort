#A helper script for installation of the TIPsort codes

#check directory the script is being run from
if [ ! -d current ]; then
	echo "This script must be run from the TIPsort base directory."
	exit
fi

#check for properly set up ROOT
echo "Checking ROOT installation..."
if [ -z "$ROOTSYS" ]
then 
		echo "ROOTSYS environment variable is not set!"
		echo "Please enter the path to the ROOT installation (base 'root' directory, which contains both 'lib' and 'include' directories): "
		read ROOTSYS
		echo "" >> ~/.bashrc
		echo "#Set up ROOT" >> ~/.bashrc
		echo "export ROOTSYS=$ROOTSYS" >> ~/.bashrc
else 
		echo "ROOTSYS environment variable is set properly."
fi

if [ -z "$ROOTINC" ]
then 
		echo "ROOTINC environment variable is not set!  Setting..."
		echo "#Set up ROOTINC" >> ~/.bashrc
		echo "export ROOTINC=\$ROOTSYS/include" >> ~/.bashrc
else 
		echo "ROOTINC environment variable is set properly."
fi

if [ -z "$ROOTLIB" ]
then 
		echo "ROOTLIB environment variable is not set!  Setting..."
		echo "#Set up ROOTLIB" >> ~/.bashrc
		echo "export ROOTLIB=\$ROOTSYS/lib" >> ~/.bashrc
else 
		echo "ROOTLIB environment variable is set properly."
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
if [ $(grep -xR "#Set up TIPsort" ~/.bashrc | wc -l) -lt 1 ]
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
