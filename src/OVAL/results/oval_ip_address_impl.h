/**
 * @file oval_ip_address.c
 * \brief Open Vulnerability and Assessment Language
 *
 * See more details at http://oval.mitre.org/
 */

/*
 * Copyright 2009--2013 Red Hat Inc., Durham, North Carolina.
 * All Rights Reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * Authors:
 *     Tomas Heinrich <theinric@redhat.com>
 *     Šimon Lukašík
 */

#ifndef OSCAP_OVAL_IP_ADDRESS_IMPL_H_
#define OSCAP_OVAL_IP_ADDRESS_IMPL_H_

#include "common/util.h"

#include "public/oval_definitions.h"
#include "public/oval_types.h"

OSCAP_HIDDEN_START;

oval_result_t ipv4addr_cmp(char *s1, char *s2, oval_operation_t op);

oval_result_t ipv6addr_cmp(char *s1, char *s2, oval_operation_t op);

OSCAP_HIDDEN_END;

#endif
