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


import os, sys, ipcalc, re, malout, malex
import xml.etree.ElementTree as ET


def check_file_exists(filename):
    if not os.path.exists(filename):
        malout.print_error("File \"%s\" cannot be found.\n" % filename)
        sys.exit()

    if not os.path.isfile(filename):
        malout.print_error("File \"%s\" is a directory or not a regular file.\n" % filename)
        sys.exit()


def check_file_path(filename, parameter):
    if not os.path.exists(filename):
        raise IOError(parameter)


def check_boolean_value(v, parameter):
    if v.upper() not in ["TRUE", "FALSE"]:
        raise malex.NotBoolean(parameter)


def check_encoding_value(v, parameter):
    if v.upper() not in ["BASE64", "BIN", "HEX"]:
        raise malex.IllegalEncoding(parameter)


def check_non_negative_value(v, parameter):
    if int(v) < 0:
        raise malex.NegativeValue(parameter)


def check_positive_value(v, parameter):
    if int(v) <= 0:
        raise malex.NonNegativeValue(parameter)


def check_legal_port(port, parameter):
    if int(port) < 1 or int(port) > 65535:
        raise malex.IllegalPort(parameter)


def check_block_size(b, parameter):
    if b not in ["16", "24", "32"]:
        raise malex.IllegalBlockSize(parameter)


def check_key_length(b, s, parameter):
    if len(s) != int(b):
        raise malex.IllegalKeySize(parameter)


def check_ip_address(ip):
    legal = False
    try:
        subnet = ipcalc.Network(ip)
        legal = True

    except ValueError:
        legal = False

    return legal


def check_legal_fqdn(fqdn):
    reg = None
    legal = False
    try:
        if len(fqdn) > 0 and len(fqdn) < 255:
            if fqdn[-1] in ["."]:
                fqdn = fqdn[:-1]

            reg = re.compile("(?!-)[A-Z\d-]{1,63}(?<!-)$", re.IGNORECASE)
            legal = all(reg.match(v) for v in fqdn.split("."))
    except:
        legal = False

    return legal


def check_host_value(h, parameter):
    legal_ip = check_ip_address(h)
    legal_fqdn = check_legal_fqdn(h)

    if not legal_ip and not legal_fqdn:
        raise malex.IllegalHostValue(parameter)


def check_legal_protocol(p, parameter):
    if p not in ["HTTP/1.0", "HTTP/1.1"]:
        raise malex.IllegalProtocol(parameter)


def check_legal_http_method(v, parameter):
    if v.upper() not in ["GET", "POST", "HEAD"]:
        raise malex.IllegalMethod(parameter)


def check_legal_parameter_location(v, parameter):
    if v.upper() not in ["QUERYSTRING", "COOKIE", "BODY"]:
        raise malex.IllegalLocation(parameter)


def check_legal_parameter_config(m, l, parameter):
    if m.upper() in ["GET", "HEAD"] and l.upper() in ["BODY"]:
        raise malex.IllegalConfig(parameter)


def get_metasploit_config(config_file):
    tree = None
    root = None
    check_file_exists(config_file)
    section = "ServerConfig -> Metasploit"

    try:
        tree = ET.parse(config_file)
        root = tree.getroot()

    except:
        malout.print_config_error(config_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        for child in root.find("Metasploit"):
            if child.text and child.tag != "msfpayload":
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, config_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, config_file))
        sys.exit()

    config = dict(config)
    try:
        if not config["path"] or not config["cachefolder"] or not config["resourcefolder"]:
            raise KeyError

        check_file_path(config["path"] + "msfvenom", "path")

    except KeyError as key:
        malout.print_config_error(config_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except IOError as i:
        malout.print_config_error(config_file, section, i, malout.PATH_MSFVENOM)
        sys.exit()

    return config


def get_payload_config(config_file):
    tree = None
    root = None
    check_file_exists(config_file)
    section = "ServerConfig -> Metasploit -> msfpayload"

    try:
        tree = ET.parse(config_file)
        root = tree.getroot()

    except:
        malout.print_config_error(config_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    tmp = []
    config = []
    try:
        msf = root.find("Metasploit")
        for child in msf.findall("msfpayload"):
            for c in child:
                tmp.append([c.tag, c.text])

            config.append(dict(tmp))

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, config_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, config_file))
        sys.exit()

    try:
        for p in config:
            if not p["id"] or not p["payload"] or not p["lhost"] or not p["lport"] or not p["encoder"] or not p["badchars"] or not p["iterations"] or not p["perpetual"]:
                raise KeyError

            check_host_value(p["lhost"], "lhost")
            check_legal_port(p["lport"], "lport")
            check_positive_value(p["iterations"], "iterations")
            check_boolean_value(p["perpetual"], "perpetual")

    except KeyError as key:
        malout.print_config_error(config_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.IllegalHostValue as v:
        malout.print_config_error(config_file, section, v, malout.INVALID_HOST)
        sys.exit()

    except malex.IllegalPort as p:
        malout.print_config_error(config_file, section, p, malout.INVALID_PORT)
        sys.exit()

    except malex.NonNegativeValue as v:
        malout.print_config_error(config_file, section, v, malout.POSITIVE_VALUE)
        sys.exit()

    except malex.NotBoolean as b:
        malout.print_config_error(config_file, section, b, malout.BOOLEAN_VALUE)
        sys.exit()

    return config


def get_scope_config(config_file):
    tree = None
    root = None
    check_file_exists(config_file)
    section = "ServerConfig -> Scope"

    try:
        tree = ET.parse(config_file)
        root = tree.getroot()

    except:
        malout.print_config_error(config_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    scope = []
    try:
        for child in root.find("Scope"):
            ip = child.text
            if ip:
                if ip.find("/") > -1:
                    for c in ipcalc.Network(ip):
                        scope.append(str(c))
                else:
                    scope.append(ip)

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, config_file))
        sys.exit()

    if not scope:
        malout.print_config_error(config_file, section, "network", malout.EMPTY_VALUE)
        sys.exit()

    return scope


def get_server_config(config_file):
    tree = None
    root = None
    check_file_exists(config_file)
    section = "ServerConfig -> Server"

    try:
        tree = ET.parse(config_file)
        root = tree.getroot()

    except:
        malout.print_config_error(config_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        for child in root.find("Server"):
            if child.text:
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, config_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, config_file))
        sys.exit()

    config = dict(config)

    try:
        if not config["threading"] or not config["addr"] or not config["port"] or not config["ssl"] or not config["sslcert"] or not config["proxy"] or not config["proxyport"] or not config["profile"] or not config["failsafe"]:
            raise KeyError

        check_host_value(config["addr"], "addr")
        check_legal_port(config["port"], "port")
        check_legal_port(config["proxyport"], "proxyport")
        check_boolean_value(config["ssl"], "ssl")
        check_boolean_value(config["proxy"], "proxy")
        check_boolean_value(config["threading"], "threading")
        check_boolean_value(config["failsafe"], "failsafe")

    except KeyError as key:
        malout.print_config_error(config_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.IllegalHostValue as v:
        malout.print_config_error(config_file, section, v, malout.INVALID_HOST)
        sys.exit()

    except malex.IllegalPort as p:
        malout.print_config_error(config_file, section, p, malout.INVALID_PORT)
        sys.exit()

    except malex.NotBoolean as b:
        malout.print_config_error(config_file, section, b, malout.BOOLEAN_VALUE)
        sys.exit()

    return config


def get_profile_request_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Server -> Request"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        req = root.find("Server").find("Request")
        for child in req:
            if child.text:
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, profile_file))
        sys.exit()

    config = dict(config)

    try:
        if not config["method"] or not config["page"] or not config["parameter"] or not config["location"]:
            raise KeyError

        check_legal_http_method(config["method"], "method")
        check_legal_parameter_location(config["location"], "location")
        check_legal_parameter_config(config["method"], config["location"], "location")

    except KeyError as key:
        malout.print_config_error(profile_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.IllegalMethod as m:
        malout.print_config_error(profile_file, section, m, malout.INVALID_METHOD)
        sys.exit()

    except malex.IllegalLocation as l:
        malout.print_config_error(profile_file, section, l, malout.INVALID_LOCATION)
        sys.exit()

    except malex.IllegalConfig as l:
        malout.print_config_error(profile_file, section, l, malout.INVALID_LOCATION_METHOD)
        sys.exit()

    return config


def get_profile_response_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Server -> Response"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        resp = root.find("Server").find("Response")
        for child in resp:
            if child.text and child.tag != "header" and child.tag != "body":
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, profile_file))
        sys.exit()

    config = dict(config)

    try:
        if not config["protocol"] or not config["banner"] or not config["lastresort"] or not config["errortemplate"]:
            raise KeyError

        check_legal_protocol(config["protocol"], "protocol")
        check_file_path(config["errortemplate"], "errortemplate")

    except KeyError as key:
        malout.print_config_error(profile_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.IllegalProtocol as p:
        malout.print_config_error(profile_file, section, p, malout.INVALID_PROTOCOL)
        sys.exit()

    except IOError as f:
        malout.print_config_error(profile_file, section, f, malout.FILE_NOT_FOUND)
        sys.exit()

    return config


def get_profile_server_header_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Server -> Response"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    tmp = []
    config = []
    try:
        resp = root.find("Server").find("Response")
        for child in resp.findall("header"):
            for c in child:
                tmp.append([c.tag, c.text])

            config.append(dict(tmp))

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    return config


def get_profile_network_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Network"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        for child in root.find("Network"):
            if child.text:
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, profile_file))
        sys.exit()

    config = dict(config)

    try:
        if not config["encoding"] or not config["rounds"] or not config["blocksize"] or not config["paddingchar"] or not config["secret"] or not config["getcaught"] or not config["reqdelay"]:
            raise KeyError

        check_encoding_value(config["encoding"], "encoding")
        check_non_negative_value(config["getcaught"], "getcaught")
        check_non_negative_value(config["reqdelay"], "reqdelay")
        check_positive_value(config["rounds"], "rounds")
        check_block_size(config["blocksize"], "blocksize")
        check_key_length(config["blocksize"], config["secret"], "secret")

    except KeyError as key:
        malout.print_config_error(profile_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.IllegalEncoding as e:
        malout.print_config_error(profile_file, section, e, malout.ENCODING_VALUE)
        sys.exit()

    except malex.NegativeValue as n:
        malout.print_config_error(profile_file, section, n, malout.NON_NEGATIVE_VALUE)
        sys.exit()

    except malex.NonNegativeValue as n:
        malout.print_config_error(profile_file, section, n, malout.POSITIVE_VALUE)
        sys.exit()

    except malex.IllegalBlockSize as b:
        malout.print_config_error(profile_file, section, b, malout.BLOCK_SIZE_VALUE)
        sys.exit()

    except malex.IllegalKeySize as k:
        malout.print_config_error(profile_file, section, k, malout.KEY_SIZE_VALUE)
        sys.exit()

    return config


def get_profile_client_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Client"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    config = []
    try:
        for child in root.find("Client"):
            if child.text:
                config.append([child.tag, child.text])

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    if not config:
        malout.print_error(malout.EMPTY_SECTION % (section, profile_file))
        sys.exit()

    config = dict(config)

    try:
        if not config["maxnaptime"]:
            raise KeyError

        check_non_negative_value(config["maxnaptime"], "maxnaptime")

    except KeyError as key:
        malout.print_config_error(profile_file, section, key, malout.EMPTY_VALUE)
        sys.exit()

    except malex.NegativeValue as n:
        malout.print_config_error(profile_file, section, n, malout.NON_NEGATIVE_VALUE)
        sys.exit()

    return config


def get_profile_client_header_config(profile_file):
    tree = None
    root = None
    check_file_exists(profile_file)
    section = "ProfileConfig -> Client"

    try:
        tree = ET.parse(profile_file)
        root = tree.getroot()

    except:
        malout.print_config_error(profile_file, section, "", malout.INVALID_CHARS)
        sys.exit()

    tmp = []
    config = []
    try:
        resp = root.find("Client")
        for child in resp.findall("header"):
            for c in child:
                tmp.append([c.tag, c.text])

            config.append(dict(tmp))

    except TypeError:
        malout.print_error(malout.NO_SECTION_FOUND % (section, profile_file))
        sys.exit()

    return config
