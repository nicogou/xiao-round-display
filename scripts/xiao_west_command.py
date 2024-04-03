'''xiao_west_command.py

Flash a Xiao BLE (Sense) with UF2 bootloader.'''

from west.commands import WestCommand  # your extension must subclass this
from west import log                   # use this for user output
import os
from sys import platform
import serial
import time

class XiaoWestFlash(WestCommand):

    def __init__(self):
        super().__init__(
            'xiao-flash',               # gets stored as self.name
            'Flash Xiao BLE Sense using UF2',  # self.help
            # self.description:
            '''\
This command checks if the F: drive is present with a uf2 file in it.
If so, copies build/zephyr/zephyr.uf2 to the drive, restarting it.
If no drive is present, checks if a serial port is ready,
and sends a command to switch it to bootloader before doing the same process.
The serial port can be specified, or a default can be used.''')

    def do_add_parser(self, parser_adder):
        # This is a bit of boilerplate, which allows you full control over the
        # type of argparse handling you want. The "parser_adder" argument is
        # the return value of an argparse.ArgumentParser.add_subparsers() call.
        parser = parser_adder.add_parser(self.name,
                                         help=self.help,
                                         description=self.description)

        # Add an option to speciofy the serial port you want to use.
        parser.add_argument('-p', '--port', help='COM port to which the Xiao is hooked on (optional)')
        parser.add_argument('-d', '--drive', help='Drive on which the bootloader hooks on on reboot (optional)')
        parser.add_argument('-b', '--build_dir', help='Name of the build folder where to look the UF2 file (defaults to build)')

        return parser           # gets stored as self.parser

    def do_run(self, args, unknown_args):
        # This gets called when the user runs the command, e.g.:
        #
        #   $ west my-command-name -o FOO BAR
        #   --optional is FOO
        #   required is BAR
        # log.inf('--optional is', args.optional)
        # log.inf('required is', args.required)
        
        if args.build_dir == None:
            args.build_dir = 'build'
            log.wrn("No build folder specified, defaulting to: \"" + args.build_dir + "\".")
        else:
            log.inf("Build folder: \"" + args.build_dir + "\"")
        
        if args.drive == None:
                args.drive = 'F:\\'
                log.wrn("No bootloader drive specified, defaulting to " + args.drive + ".")
        else:
            args.drive = args.drive + '\\'
        
        skip = False
        if os.path.exists(os.path.join(args.drive, 'CURRENT.UF2')):
            # Xiao already in bootloader mode. Skipping switching it to bootloader
            skip = True
        
        if skip == False:
            if args.port == None:
                args.port = "COM9"
                log.wrn("No serial port specified, defaulting to " + args.port + ".")

            port_open = True
            try:
                serialPort = serial.Serial(port=args.port, baudrate=115200, bytesize=8, timeout=2, stopbits=serial.STOPBITS_ONE)
            except serial.serialutil.SerialException as e:
                z = e
                log.err(args.port + " not open. Make sure Xiao serial port is open before flashing.")
                port_open = False
            if port_open:
                log.inf("Switching Xiao BLE Sense to Bootloader mode...")
                serialPort.write(b"Switch to Bootloader\r\n")
                count = 0
                while (not os.path.exists(os.path.join(args.drive, 'CURRENT.UF2')) and count < 10):
                    time.sleep(0.5)
                    count+=1
                    
        source = os.path.join(os.getcwd(), 'app', args.build_dir,
                              'zephyr', 'zephyr.uf2')
        dest = os.path.join(args.drive)

        if os.path.exists(os.path.join(args.drive, 'CURRENT.UF2')):
            log.inf('Xiao BLE Sense in bootloader mode')
            if os.path.exists(os.path.join(os.getcwd(), 'app', 'build', 'zephyr', 'zephyr.uf2')):
                log.inf('Flashing Xiao BLE Sense...')
                if platform == 'linux':
                    os.system('cp ' + source + ' ' + dest)
                elif platform == 'win32':
                    os.system('copy ' + source + ' ' + dest)
                log.inf('Flashing done')
            else:
                log.err('zephyr.uf2 file not found, unable to flash')
                return
        else:
            log.err('Xiao BLE Sense not in bootloader mode, unable to flash')
            return
