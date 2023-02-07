# www.lindwurm-can.org
# Copyright (C) 2023 Sascha Muenzberg <sascha@lindwurm-can.org>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Affero General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Affero General Public License for more details.
#
# You should have received a copy of the GNU Affero General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

DESTDIR = ../../../bin/plugins

include(../pluginsystem/pluginsystem.pri)

# plugins are generally generated without soname, but because coreplugin itself is a library for other plugins
# the loader does not find the coreplugin library when loading the others unless you add the "plugins/" directory to LD_LIBRARY_PATH
# if we build plugins with a soname, the loader recognizes that the coreplugin library is already loaded when loading other plugins

# TODO: Check behaviour on windows (mac os)
CONFIG += plugin_with_soname
