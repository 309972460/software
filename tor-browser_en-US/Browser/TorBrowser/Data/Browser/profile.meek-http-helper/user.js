// http://kb.mozillazine.org/User.js_file

// The meek-http-helper extension uses dump to write its listening port number
// to stdout.
user_pref("browser.dom.window.dump.enabled", true);

// Enable TLS session tickets (disabled by default in Tor Browser). Otherwise
// there is a missing TLS extension.
// https://trac.torproject.org/projects/tor/ticket/13442#comment:1
user_pref("security.ssl.disable_session_identifiers", false);

// Disable safe mode. In case of a crash, we don't want to prompt for a
// safe-mode browser that has extensions disabled and no proxy.
// https://support.mozilla.org/en-US/questions/951221#answer-410562
user_pref("toolkit.startup.max_resumed_crashes", -1);

// Set a failsafe blackhole proxy of 127.0.0.1:9, to prevent network interaction
// in case the user manages to open this profile with a normal browser UI (i.e.,
// not headless with the meek-http-helper extension running). Port 9 is
// "discard", so it should work as a blackhole whether the port is open or
// closed. network.proxy.type=1 means "Manual proxy configuration".
// http://kb.mozillazine.org/Network.proxy.type
user_pref("network.proxy.type", 1);
user_pref("network.proxy.socks", "127.0.0.1");
user_pref("network.proxy.socks_port", 9);
// Make sure DNS is also blackholed. network.proxy.socks_remote_dns is
// overridden by meek-http-helper at startup.
user_pref("network.proxy.socks_remote_dns", true);

user_pref("extensions.enabledAddons", "meek-http-helper@bamsoftware.com:1.0");
