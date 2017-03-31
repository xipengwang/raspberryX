This is a project for fun and for future use.

Wiki: https://github.com/xipengwang/X/wiki

***
Basic installation steps: (***You will find more information on the [wiki](https://github.com/xipengwang/X/wiki) pages***)
* `git clone git@github.com:xipengwang/X.git robotX`
*  `cd robotX` 
* `make`
    * If you compile the code on a raspberry pi, you should change `Build.common` file. 
        * Change `arm-linux-gnueabi-gcc` to `gcc`,  `arm-linux-gnueabi-ar` to `ar` 
        
    * If you compile the code on a laptop running linux, you can install cross-compiler. Then send binary files to the pi. 
        * `sudo apt-get install gcc-arm-linux-gnueabi`
        * `make`
        * `./scrpit/push_to_robot.sh` 
***
Old Wiki: https://bitbucket.org/xipengw/wiki/wiki/Home

RasPi Peripherals datasheet: https://www.raspberrypi.org/wp-content/uploads/2012/02/BCM2835-ARM-Peripherals.pdf

