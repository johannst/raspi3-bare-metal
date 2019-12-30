import os
import struct
import argparse
import time
import serial
import logging

class Flasher:
    def __init__(self, dev):
        self.dev = serial.Serial(port=args.dev,
                                 baudrate=115200,
                                 bytesize=serial.EIGHTBITS,
                                 parity=serial.PARITY_NONE,
                                 stopbits=serial.STOPBITS_ONE,
                                 #timeout=5,      # read timeout
                                 )
        self.start_seq = b"pi3loader start"
        self.stop_seq = b"pi3loader done"

        logging.info("opening serial device %s" % dev)
        logging.info("   baudrate: %d" % self.dev.baudrate)
        logging.info("   bytesize: %s" % self.dev.bytesize)
        logging.info("   stopbits: %s" % self.dev.stopbits)
        logging.info("   parity  : %s" % self.dev.parity)

    def flash(self, image):
        image_size = os.stat(image).st_size
        logging.info("image %s (%d bytes)" % (image, image_size))

        # wait loader ready
        logging.info("wait for loader start sequence ...")
        self.dev.read_until(self.start_seq + b"\r\n")
        logging.info("loader ready to receive data, flashing ...")

        image_size = struct.pack('<I', image_size)
        self.dev.write(image_size)

        # send image
        with open(image, 'rb') as image:
            byte = image.read(1)
            while byte:
                self.dev.write(byte)
                byte = image.read(1)
                # FIXME: cheap hack because of lack of flow control
                time.sleep(0.001)

        # check success
        r = self.dev.read_until(self.stop_seq + b"\r\n")
        logging.info('flashing done received stop sequence "%s"' % r.decode('ascii').strip())

        # 'n': fail
        # 'y': success
        success = self.dev.read(1)
        if success == b'y':
            logging.info("SUCCESS")
        else:
            logging.error("FAILED")

# config logger
logging.basicConfig(format='[%(levelname)s] %(filename)s: %(message)s',
                    level=logging.INFO,
                   )

# cmdline args
parser = argparse.ArgumentParser(description='Simple utility to flash image onto raspberry pi via a serial port.')
parser.add_argument('--dev', '-d',
                    dest='dev',
                    default='/dev/ttyUSB0',
                    type=str,
                    help='Serial tty connected to UART (default: /dev/ttyUSB0)')
parser.add_argument('--img', '-i',
                    dest='img',
                    default='kernel8.img',
                    type=str,
                    help='Image to be flashed (default: kernel8.img)')
args = parser.parse_args()

# flash
flasher = Flasher(args.dev)
flasher.flash(args.img)

