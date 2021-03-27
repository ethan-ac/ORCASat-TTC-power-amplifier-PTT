#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: Amplifier PTT
# Author: ethan
# GNU Radio version: v3.8.2.0-79-gd56bdd16

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from gnuradio import blocks
from gnuradio import gr
from gnuradio.filter import firdes
import sys
import signal
from PyQt5 import Qt
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
import ampkey

from gnuradio import qtgui

class ptt_hier(gr.top_block, Qt.QWidget):

    def __init__(self, mute=False, post_tx=50, pre_tx=50, samp_rate_usrp=2000000):
        gr.top_block.__init__(self, "Amplifier PTT")
        Qt.QWidget.__init__(self)
        self.setWindowTitle("Amplifier PTT")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "ptt_hier")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Parameters
        ##################################################
        self.mute = mute
        self.post_tx = post_tx
        self.pre_tx = pre_tx
        self.samp_rate_usrp = samp_rate_usrp

        ##################################################
        # Blocks
        ##################################################
        self.blocks_mute_xx_0 = blocks.mute_cc(bool(mute))
        self.ampkey_samp_delay_0 = ampkey.samp_delay(gr.sizeof_gr_complex*1, pre_tx, samp_rate_usrp)
        self.ampkey_keyer_0 = ampkey.keyer(gr.sizeof_gr_complex*1, pre_tx, post_tx)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.ampkey_samp_delay_0, 0), (self, 0))
        self.connect((self.blocks_mute_xx_0, 0), (self.ampkey_keyer_0, 0))
        self.connect((self.blocks_mute_xx_0, 0), (self.ampkey_samp_delay_0, 0))
        self.connect((self, 0), (self.blocks_mute_xx_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "ptt_hier")
        self.settings.setValue("geometry", self.saveGeometry())
        event.accept()

    def get_mute(self):
        return self.mute

    def set_mute(self, mute):
        self.mute = mute
        self.blocks_mute_xx_0.set_mute(bool(self.mute))

    def get_post_tx(self):
        return self.post_tx

    def set_post_tx(self, post_tx):
        self.post_tx = post_tx

    def get_pre_tx(self):
        return self.pre_tx

    def set_pre_tx(self, pre_tx):
        self.pre_tx = pre_tx

    def get_samp_rate_usrp(self):
        return self.samp_rate_usrp

    def set_samp_rate_usrp(self, samp_rate_usrp):
        self.samp_rate_usrp = samp_rate_usrp




def argument_parser():
    parser = ArgumentParser()
    parser.add_argument(
        "--post-tx", dest="post_tx", type=eng_float, default="50.0",
        help="Set Post Tx Delay (ms) [default=%(default)r]")
    parser.add_argument(
        "--pre-tx", dest="pre_tx", type=eng_float, default="50.0",
        help="Set Pre Tx Delay (ms) [default=%(default)r]")
    parser.add_argument(
        "--samp-rate-usrp", dest="samp_rate_usrp", type=intx, default=2000000,
        help="Set Sample Rate [default=%(default)r]")
    return parser


def main(top_block_cls=ptt_hier, options=None):
    if options is None:
        options = argument_parser().parse_args()

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls(post_tx=options.post_tx, pre_tx=options.pre_tx, samp_rate_usrp=options.samp_rate_usrp)

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    def quitting():
        tb.stop()
        tb.wait()

    qapp.aboutToQuit.connect(quitting)
    qapp.exec_()

if __name__ == '__main__':
    main()
