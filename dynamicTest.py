#!/usr/bin/python

import argparse, sys, os, subprocess

BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE = range(8)

#following from Python cookbook, #475186
def has_colours(stream):
    if not hasattr(stream, "isatty"):
        return False
    if not stream.isatty():
        return False # auto color only on TTYs
    try:
        import curses
        curses.setupterm()
        return curses.tigetnum("colors") > 2
    except:
        # guess false in case of error
        return False
has_colours = has_colours(sys.stdout)


def printout(text, colour=WHITE):
    if has_colours:
        seq = "\x1b[1;%dm" % (30+colour) + text + "\x1b[0m"
        sys.stdout.write(seq)
    else:
        sys.stdout.write(text)

if __name__ == '__main__':

    parser = argparse.ArgumentParser(description='Tests for Pricer 2014')
    subparsers = parser.add_subparsers()

    # Price option
    parser_price_option = subparsers.add_parser('price_option', help='Test the option pricer')
    parser_price_option.set_defaults(which='parser_price_option')
    parser_price_option.add_argument('-f','--file', required=True, type=argparse.FileType('r'))
    parser_price_option.add_argument('-t','--time', type = float, default = '-1')

    # Compute delta
    parser_compute_delta = subparsers.add_parser('compute_delta', help='Compute the delta of an option')
    parser_compute_delta.set_defaults(which='parser_compute_delta');
    parser_compute_delta.add_argument('-f','--file', required=True, type=argparse.FileType('r'))
    parser_compute_delta.add_argument('-t','--time', type = float, default = '-1')

    # Monte carlo
    parser_monte_carlo = subparsers.add_parser('monte_carlo', help='Test monte carlo')
    parser_monte_carlo.set_defaults(which='parser_monte_carlo');
    parser_monte_carlo.add_argument('-f','--file', required=True, type=argparse.FileType('r'))

    # Black Scholes
    parser_bs = subparsers.add_parser('black_scholes', help='Test black scholes')
    parser_bs.set_defaults(which='parser_bs');
    parser_bs.add_argument('-f','--file', required=True, type=argparse.FileType('r'))

    args = parser.parse_args()

    if (args.which == 'parser_price_option'):
        if (not os.path.exists("build/price-option")):
            printout('Could not find build/price-option, build the project before using this script\n', RED);
        subprocess.call(['build/price-option',args.file.name, str(args.time)])
    elif (args.which == 'parser_compute_delta'):
        if (not os.path.exists("build/compute-delta")):
            printout('Could not find build/compute-delta, build the project before using this script\n', RED);
        subprocess.call(['build/compute-delta',args.file.name, str(args.time)])
    elif  (args.which == 'parser_monte_carlo'):
        pass
    elif  (args.which == 'parser_bs'):
        pass
