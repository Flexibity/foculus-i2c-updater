/**
 * File: main.c
 * Created on: 13 марта 2015 г.
 * Description:
 * 
 *
 * Author: Roman Savrulin <romeo.deepmind@gmail.com>
 * Copyright: 2014 Roman Savrulin
 * Copying permission statement:
 This file is part of foculus-i2c-updater.

 foculus-i2c-updater is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 */

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Based on info from John Newbigin jnewbigin@chrysocome.net http://blog.chrysocome.net/2013/03/programming-i2c.html

int main(int argc, char **argv) {
  int i;
  int r;
  int fd;
  unsigned char command[2];
  unsigned char value[4];
  useconds_t delay = 2000;
  int bytesToRead = 256;

  char *dev = "/dev/i2c-1";
  int addr = 0x50;

  printf("oculus-i2c-updater\n");

  fd = open(dev, O_RDWR);
  if (fd < 0) {
    perror("Opening i2c device node\n");
    return 1;
  }

  r = ioctl(fd, I2C_SLAVE, addr);
  if (r < 0) {
    perror("Selecting i2c device\n");
  }

  while (1) {
    for (i = 0; i < bytesToRead / 16; i++) {
      for(int j = 0; j < 16; j++){
        command[0] = 0x40 | ((i + 1) & 0x03); // output enable | read input i
        //command[1] = ;
        r = write(fd, &command, 2);
        usleep(delay);
        // the read is always one step behind the selected input
        r = read(fd, &value[0], 1);
        if (r != 1) {
          perror("reading i2c device\n");
        }
        usleep(delay);
        printf("0x%02x ", value[0]);
      }
    }
  }

  close(fd);
  return (0);
}
