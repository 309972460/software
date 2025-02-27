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

import os, sys, httplib, subprocess, urllib
import malout
from urlparse import urlparse, parse_qs
from datetime import datetime


def get_payload_id_from_path(path, param):
    payload_id = None
    if path:
        path = urllib.unquote(path)
        if path.find("=") > -1:
            query = urlparse(path).query
            url_params = dict(q.split("=") for q in query.split("&"))
            payload_id = url_params.get(param)

    return payload_id


def get_payload_id_from_body(body, param):
    payload_id = None
    if body:
        body = urllib.unquote(body)
        if body.find("=") > -1:
            body_params = dict(q.split("=") for q in body.split("&"))
            payload_id = body_params.get(param)

    return payload_id


def get_payload_id_from_cookie(cookie, param):
    payload_id = None
    if cookie:
        cookie = urllib.unquote(cookie).replace("\n", "").replace(" ", "")
        if cookie.find("=") > -1:
            cookie_params = dict(q.split("=") for q in cookie.split(";"))
            payload_id = cookie_params.get(param)

    return payload_id


def get_cache_file_path(payload_id, folder_name, default_file):
    cache_file = str(payload_id)
    cache_path = "%s/%s" % (folder_name, cache_file)
    if os.path.isfile(cache_path):
        malout.print_ok("Requested payload OK...")

    # Uses the first (default) generated payload if not found
    else:
        cache_path = "%s/%s" % (folder_name, default_file)
        malout.print_warning("Requested payload not found in cache. Using default payload instead...")

    return cache_path


def start_socks_proxy(metasploit_config, port):
    try:
        resource_folder = metasploit_config["resourcefolder"]
        resource_file = "socks4a.rc"

        if not os.path.exists(resource_folder):
            os.makedirs(resource_folder)

        resource_path = "%s/%s" % (resource_folder, resource_file)
        fw = open(resource_path, 'w')

        fw.write("use auxiliary/server/socks4a\n")
        fw.write("set SRVPORT " + port + "\n")
        fw.write("set VERBOSE true\n")
        fw.write("run\n")
        fw.close()

    except IOError:
        libs.malout.print_error("Could not generate socks4a resource file.")

    except OSError:
        libs.malout.print_error("Could not create metasploit resource folder.")

    msf_path = metasploit_config["path"]
    p = subprocess.Popen("%smsfconsole -q -r %s" % (msf_path, resource_path), stdout=subprocess.PIPE, shell=True)

    return p


def start_msfconsole(metasploit_config, resource_file):
    p = subprocess.Popen("gnome-terminal -e \"bash -c 'echo Starting...;%smsfconsole -q -r ./%s/%s'\"" % (metasploit_config["path"], metasploit_config["resourcefolder"], resource_file), shell=True)

    return p


def check_for_updates(version):
    try:
        http_conn = httplib.HTTPConnection("www.encripto.no", 80, timeout=10)
        headers = { "User-Agent" : "Mozilla/5.0 (compatible; Maligno-Srv/" + version + ")"}
        http_conn.request("HEAD", "/tools/maligno-" + version + ".tar.gz", None, headers)
        http_resp = http_conn.getresponse()

        if http_resp.status == 404:
            print ""
            malout.print_ok("There is a new version of Maligno!")
            malout.print_ok("Check http://www.encripto.no/tools for more information.\n")

        elif http_resp.status == 200:
            malout.print_ok("You are running the latest version of Maligno.\n")

        http_conn.close()

    except:
        malout.print_error("Could not check for updates...\n")

    return

