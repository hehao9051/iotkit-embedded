/*
 * Copyright (C) 2015-2018 Alibaba Group Holding Limited
 */

#include "iotx_alink_internal.h"
#include "alink_wrapper.h"


#define _ALINK_EXTEND_STRING_SPLIT_DELIMITER    "?"
#define _ALINK_EXTEND_STRING_AND_DELIMITER      "&"

#define _ALINK_EXTEND_STRING_ACK                "a"     /* shortname of ack */
#define _ALINK_EXTEND_STRING_CODE               "c"     /* shortname of code */
#define _ALINK_EXTEND_STRING_FORMAT             "f"     /* shortname of format */
#define _ALINK_EXTEND_STRING_ID                 "i"     /* shortname of id */
#define _ALINK_EXTEND_STRING_COMPRESSION        "m"     /* shortname of compression */


#define ALINK_URI_DIST_CLOUD        0x00
#define ALINK_URI_DIST_APP          0x10
#define ALINK_URI_DIST_DEVICE       0x20

#define ALINK_URI_ACT_REQ           0x00
#define ALINK_URI_ACT_RSP           0x01

#define ALINK_URI_LAYER_SYS         0x00
#define ALINK_URI_LAYER_EXT         0x10
#define ALINK_URI_LAYER_PROXY       0x20

#define ALINK_URI_METHOD_GET        0x00
#define ALINK_URI_METHOD_POST       0x01
#define ALINK_URI_METHOD_PUT        0x02
#define ALINK_URI_METHOD_DELETE     0x03

typedef struct {
    uint8_t     dist_act;
    uint8_t     layer_method;
    const char *path;
} alink_uri_string_map_t;

const char *alink_uri_dist[] = {
    "/c/iot",
    "/app/%s",
    "/%s/%s"
};

const char *alink_uri_act[] = {
    "/req",
    "/rsp"
};

const char *alink_uri_layer[] = {
    "/sys",
    "/ext",
    "/proxy"
};

const char *alink_uri_method[] = {
    "/get",
    "/post",
    "/put",
    "/delete"
};

const alink_uri_string_map_t c_alink_uri_string_map[] = {
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/thing/property"       },  /* "/c/iot/req/sys/thing/property/post" */
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/thing/property"       },  /* "/c/iot/rsp/sys/thing/property/put" */
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_GET),       "/thing/property"       },  /* "/c/iot/rsp/sys/thing/property/get" */
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/thing/event"          },  /* "/c/iot/req/sys/thing/event/post" */
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/thing/service"        },  /* "/c/iot/rsp/sys/thing/service/put" */

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/thing/raw"            },  /* "/c/iot/req/sys/thing/raw/post" */
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/thing/raw"            },  /* "/c/iot/rsp/sys/thing/raw/post" */

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/sub/register"         },
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_DELETE),    "/sub/register"         },

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/sub/login"            },  
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_DELETE),    "/sub/login"            },  
    
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/thing/topo"           }, 
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_DELETE),    "/thing/topo"           },  
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_GET),       "/thing/topo"           }, 

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/sub/list"             },  
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/sub/list"             },  

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/gw/permit"            },  
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_RSP),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_PUT),       "/gw/config"            },  

    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_POST),      "/thing/devinfo"        }, 
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_GET),       "/thing/devinfo"        }, 
    { (ALINK_URI_DIST_CLOUD | ALINK_URI_ACT_REQ),   (ALINK_URI_LAYER_SYS | ALINK_URI_METHOD_DELETE),    "/thing/devinfo"        }, 
};

/**
 *
 */
const char *alink_msg_uri_short_alias[] = {
    "1",            /* property post request */
    "2",            /* property set response */
    "3",            /* porperty get response */
    "4",            /* event post request */
    "5",            /* service post response */
    "6",            /* raw data upstream request */
    "7",            /* raw data downstream response */
    "8",            /* subdev reigister */
    "9",            /* subdev unregister */
    "10",           /* subdev add topo */
};

/**********************************
 * local function
 **********************************/


/**********************************
 * global function
 **********************************/
int alink_format_get_upstream_complete_uri(alink_msg_uri_index_t index, const char *uri_query, char **p_uri)
{
    //int res = FAIL_RETURN;
    uint16_t len;

    const char *uri_dist = alink_uri_dist[(c_alink_uri_string_map[index].dist_act >> 4)];
    const char *uri_act = alink_uri_act[(c_alink_uri_string_map[index].dist_act & 0x0F)];
    const char *uri_layer = alink_uri_layer[(c_alink_uri_string_map[index].layer_method >> 4)];
    const char *uri_path = c_alink_uri_string_map[index].path;
    const char *uri_method = alink_uri_method[(c_alink_uri_string_map[index].layer_method & 0x0F)];

    len = strlen(uri_dist) + strlen(uri_act) + strlen(uri_layer) + strlen(uri_path) + strlen(uri_method) + strlen(uri_query) + 1;

    char *uri = alink_malloc(len);
    if (uri == NULL) {
        return FAIL_RETURN;
    }

    HAL_Snprintf(uri, len, "%s%s%s%s%s%s", uri_dist, uri_act, uri_layer, uri_path, uri_method, uri_query);

    *p_uri = uri;

    return SUCCESS_RETURN;
}

/**
 * /c/iot/req/proxy/{subdev-productkey}/{subdev-devicename}/sys/thing/property/post
 */
int alink_format_get_upstream_subdev_complete_url(alink_msg_uri_index_t index, const char *subdev_pk, const char *subdev_dn, const char *uri_query, char **p_uri)
{
    ALINK_ASSERT_DEBUG(index < ALINK_URI_UP_MAX && index >= 0 );
    ALINK_ASSERT_DEBUG(subdev_pk != NULL);
    ALINK_ASSERT_DEBUG(subdev_dn != NULL);
    ALINK_ASSERT_DEBUG(uri_query != NULL);
    ALINK_ASSERT_DEBUG(p_uri != NULL);

    uint16_t len;

    const char *uri_dist = alink_uri_dist[(c_alink_uri_string_map[index].dist_act >> 4)];
    const char *uri_act = alink_uri_act[(c_alink_uri_string_map[index].dist_act & 0x0F)];
    const char *uri_layer = alink_uri_layer[(c_alink_uri_string_map[index].layer_method >> 4)];
    const char *uri_path = c_alink_uri_string_map[index].path;
    const char *uri_method = alink_uri_method[(c_alink_uri_string_map[index].layer_method & 0x0F)];

    len = strlen(uri_dist) + strlen(uri_act) + strlen(uri_layer) + strlen(uri_path) + strlen(uri_method) 
        + strlen(uri_query) + strlen(subdev_pk) + strlen(subdev_dn) + strlen(alink_uri_layer[ALINK_URI_LAYER_PROXY >> 4]) + 3;    /* add 2 "/" delimiter */

    char *uri = alink_malloc(len);
    if (uri == NULL) {
        return FAIL_RETURN;
    }

    alink_info("query len = %d", strlen(uri_query));
    alink_info("len = %d", len);

    /* e... - -! */
    HAL_Snprintf(uri, len, "%s%s%s/%s/%s%s%s%s%s", uri_dist, uri_act, 
                alink_uri_layer[ALINK_URI_LAYER_PROXY >> 4], subdev_pk, subdev_dn, uri_layer, uri_path, uri_method, uri_query);

    *p_uri = uri;

    return SUCCESS_RETURN;
}

/**
 * 
 */
const char *alink_format_get_upstream_alias_uri(alink_msg_uri_index_t index)
{
    return alink_msg_uri_short_alias[index];
}

/**
 * 
 */
int alink_format_assemble_query(alink_uri_query_t *query, char *query_string)
{
    if (query->ack == '\0') {
        ;
    }


    return SUCCESS_RETURN;
}

// TODO: not good enough!!!
int alink_format_resolve_query(const char *uri, uint8_t *uri_len, alink_uri_query_t *query)
{
    const char *p = uri + *uri_len;
    uint8_t len = 0;
    uint8_t i = 0;
    char temp[30] = {0};        // TODO, malloc

    while (--p != uri) {
        len++;
        if (*p == '?') {
            break;
        }
        else if (*p == '/') {
            return FAIL_RETURN;
        }
    }
    
    if (len >= 30) {      // TODO
        return FAIL_RETURN;
    }

    *uri_len -= (len+1);    // query_len not include '/'

    memcpy(temp, p, len);
    alink_info("query = %s", temp);

    while (i++ < len) {
        switch (temp[i]) {
            case 'f': {
                i += 2;
                query->format = temp[i]; 
            } break;
            case 'i': {
                i += 2;
                query->id = atoi(temp+i);
            } break;
            case 'c': {
                i += 2;
                query->compress = temp[i]; 
            } break;
            case 'a': {
                i += 2;
                query->ack =  temp[i]; 
            } break;
            case 'r': {
                i += 2;
                query->code = atoi(&temp[i]);     // tood, atoi() used!!!
            }
            default: continue;
        }
    }

    // set default value if absence
    if (query->ack == '\0') {
        query->ack = 'y';
    }

    return 0;
}

/**
 * no include char '/', level start from 1, not support the last level, todo!!!
 */
int _alink_get_uri_level_value(const char *uri, uint8_t uri_len, uint8_t level, char *value)
{
    ALINK_ASSERT_DEBUG(uri != NULL);
    ALINK_ASSERT_DEBUG(uri_len != 0);
    ALINK_ASSERT_DEBUG(value != NULL);

    uint8_t idx = 0;
    uint8_t cnt = 0;
    char *p1 = NULL;
    char *p2 = NULL;

    for (idx = 0; idx < uri_len; idx++) {
        if (*(uri+idx) == '/' && (idx + 1) < uri_len) {
            cnt++;
            if (cnt == level) {
                p1 = (char *)(uri + idx + 1);
            } 
            else if (cnt == level+1) {
                p2 = (char *)(uri + idx + 1);
            }
        }
    }

    if (!p1 || !p2 ) {
        return FAIL_RETURN;
    }    

    memcpy(value, p1, p2-p1-1); 
    value[p2-p1-1] = 0;

    return SUCCESS_RETURN;
}


/**
 * not include '/'
 */
int _alink_get_uri_level_pointer(const char *uri, uint8_t uri_len, uint8_t level, char **p_value)
{
    ALINK_ASSERT_DEBUG(uri != NULL);
    ALINK_ASSERT_DEBUG(uri_len != 0);
    ALINK_ASSERT_DEBUG(p_value != NULL);

    uint8_t idx = 0;
    uint8_t cnt = 0;

    for (idx = 0; idx < uri_len; idx++) {
        if (*(uri+idx) == '/' && (idx + 1) < uri_len) {
            cnt++;
            if (cnt == level) {
                *p_value = (char *)(uri + idx + 1);

                return SUCCESS_RETURN;
            }
        }
    }

    return FAIL_RETURN;
}

/**
 * TODO, not good!!!
 */
int alink_format_reslove_uri(const char *uri, uint8_t uri_len, char *pk, char *dn, char *path, alink_uri_query_t *query, uint8_t *is_subdev)
{
    char value[33] = {0};
    uint8_t value_len = 0;
    char *p = NULL;

    uint8_t uri_len_temp = uri_len;


    alink_format_resolve_query(uri, &uri_len_temp, query);


    /* is proxy */
    _alink_get_uri_level_value(uri, uri_len_temp, 4, value);
    value_len = strlen(value);

    if (value_len == strlen("proxy") && !memcmp(value, "proxy", value_len))
    {
        *is_subdev = IOT_TRUE;

        _alink_get_uri_level_value(uri, uri_len_temp, 5, pk);
        _alink_get_uri_level_value(uri, uri_len_temp, 6, dn);

        _alink_get_uri_level_value(uri, uri_len_temp, 3, path);
        memcpy(path+strlen(path), "/", 1);

        _alink_get_uri_level_pointer(uri, uri_len_temp, 7, &p);
        memcpy(path+strlen(path), p, (uri_len_temp - (uint8_t)(p - uri)));
    }
    else {
        *is_subdev = IOT_FALSE;

        _alink_get_uri_level_value(uri, uri_len_temp, 1, pk);
        _alink_get_uri_level_value(uri, uri_len_temp, 2, dn);

        _alink_get_uri_level_pointer(uri, uri_len_temp, 3, &p);
        memcpy(path, p, (uri_len_temp - (uint8_t)(p - uri)));
    }

    

    return SUCCESS_RETURN;
}
