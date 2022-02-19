1. Forked esp-hosted solution from expressif
2. To build for Rpi: a. Clone raspberry pi linux git on WSL machine
                     b. Build the kernel : 
                                          steps:  /*https://www.raspberrypi.com/documentation/computers/linux_kernel.html#cross-compiling-the-kernel*/
					  i. Download arm toolchain arm-linux-gnueabihf-
												   cd linux
												   KERNEL=kernel
												   make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- bcmrpi_defconfig
												   
				         ii. make ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- zImage modules dtbs
												
					/*we will need to generate a directory containing kernel modules that Raspberry PI can load when needed. 
					Run the following command to copy the modules to the ‘../modules’ directory*/
												
					iii.ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf- INSTALL_MOD_PATH=../modules make modules_install
												
					/*cd to modules directory created earlier*/
												
					iv. tar czf modules.tgz *
												
					/*git clone https://github.com/raspberrypi/tools
					./imagetool-uncompressed.py ../../linux/arch/arm/boot/zImage 
					kernel.img is generated*/
												
					SSH to rpi
					v. sudo mv <PATH TO>kernel.img /boot/ 
					   sudo tar xzf <PATH TO>modules.tgz 

3. Building ESP-Hosted solution on WSL (only driver part:modified the MAKEFILE to build the driver)
4. Copy driver to RPI 
