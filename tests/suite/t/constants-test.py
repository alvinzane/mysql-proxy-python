# LICENSE BEGIN
#
# Copyright (c) 2010 Ysj.Ray
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
## LICENSE END


import os, sys
sys.path.append(os.environ['PYTHON_LIBPATH'])

import re

import chassis

#- MYSQL_PROXY_VERSION is something like 0.8.0
v = os.environ["MYSQL_PROXY_VERSION"]

def read_query(proxy, packet):
    if ord(packet[0]) != proxy.COM_QUERY:
        proxy.response = {
            'type' : proxy.MYSQLD_PACKET_OK
        }
        return proxy.PROXY_SEND_RESULT

    #--
    #- extract the version string by hand and build it ourself
    #-
    #- 0.8.0 -> 0x00 0x08 0x00
    #-
    #- if a assert()ion fails the proxy will just forward the query to the backend
    #- we will return "failed"
    s_maj, s_min, s_patch = re.match("^(\d+)\.(\d+)\.(\d+)", v).groups()

    maj,mini,patch = int(s_maj), int(s_min), int(s_patch)

    assert maj >= 0
    assert mini > 0
    assert mini < 100
    assert patch >= 0
    assert patch < 100

    proxy_version = maj * 65536 + mini * 256 + patch

    assert proxy.PROXY_VERSION == '0.7.0'
    assert v == '0.8.0'

    proxy.response = {
        'type' : proxy.MYSQLD_PACKET_OK,
        'resultset' : {
            'fields' : (
                ( "Result", proxy.MYSQL_TYPE_STRING ),
            ),
            'rows' : ( ( "passed", ),  )
        }
    }

    return proxy.PROXY_SEND_RESULT
