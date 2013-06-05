#ifndef _LUAVITON_SSL_HEAD_
#define _LUAVITON_SSL_HEAD_

#include "graviton.hpp"
#include "core/luaviton.hpp"
#include "core/luaviton_socket.hpp"
#include <external/luasec.hpp>

namespace GraVitoN
{

namespace Core
{

// #ifdef GVN_DEBUG
// #endif

class Luaviton_SSL : public Luaviton_Module<Luaviton_SSL>
{
protected:
    string MODULE_SSL;
    string MODULE_HTTPS;

public:
    Luaviton_SSL ();//(Luaviton &_luaviton_instance);

    virtual ~Luaviton_SSL() {}

    void registerModule()
    {
        Luaviton_Socket::getInstance().registerModule();

        GraVitoN::Core::Logger::logItLn("Loading Module: ssl.core");
        luaviton.preloadModule("ssl.core", luaopen_ssl_core);
        GraVitoN::Core::Logger::logItLn("Loading Module: ssl.context");
        luaviton.preloadModule("ssl.context", luaopen_ssl_context);
    }

    bool loadEmAll()
    {
        if( !Luaviton_Socket::getInstance().loadEmAll() )
        {
            Logger::logItLn("[Lua SSL Faild] Cannot load Lua Socket");
            return false;
        }

        GraVitoN::Core::Logger::logItLn("Loading Module: ssl");
        GraVitoN::Core::Logger::logItLn("Loading Module: https");
        return
                luaviton.loadModuleString(MODULE_SSL) &&
                luaviton.loadModuleString(MODULE_HTTPS);
    }
};

Luaviton_SSL::Luaviton_SSL () //(Luaviton &_luaviton_instance) : Luaviton_Socket(_luaviton_instance)
{
    MODULE_SSL =
            "------------------------------------------------------------------------------\n"
            "-- LuaSec 0.4.1\n"
            "-- Copyright (C) 2006-2011 Bruno Silvestre\n"
            "--\n"
            "------------------------------------------------------------------------------\n"
            "\n"
            "module(\"ssl\", package.seeall)\n"
            "\n"
            "require(\"ssl.core\")\n"
            "require(\"ssl.context\")\n"
            "\n"
            "\n"
            "_VERSION   = \"0.4.1\"\n"
            "_COPYRIGHT = \"LuaSec 0.4.1 - Copyright (C) 2006-2011 Bruno Silvestre\\n\" ..\n"
            "             \"LuaSocket 2.0.2 - Copyright (C) 2004-2007 Diego Nehab\"\n"
            "\n"
            "-- Export functions\n"
            "rawconnection = core.rawconnection\n"
            "rawcontext    = context.rawcontext\n"
            "\n"
            "--\n"
            "--\n"
            "--\n"
            "local function optexec(func, param, ctx)\n"
            "  if param then\n"
            "    if type(param) == \"table\" then\n"
            "      return func(ctx, unpack(param))\n"
            "    else\n"
            "      return func(ctx, param)\n"
            "    end\n"
            "  end\n"
            "  return true\n"
            "end\n"
            "\n"
            "--\n"
            "--\n"
            "--\n"
            "function newcontext(cfg)\n"
            "   local succ, msg, ctx\n"
            "   -- Create the context\n"
            "   ctx, msg = context.create(cfg.protocol)\n"
            "   if not ctx then return nil, msg end\n"
            "   -- Mode\n"
            "   succ, msg = context.setmode(ctx, cfg.mode)\n"
            "   if not succ then return nil, msg end\n"
            "   -- Load the key\n"
            "   if cfg.key then\n"
            "      succ, msg = context.loadkey(ctx, cfg.key, cfg.password)\n"
            "      if not succ then return nil, msg end\n"
            "   end\n"
            "   -- Load the certificate\n"
            "   if cfg.certificate then\n"
            "      succ, msg = context.loadcert(ctx, cfg.certificate)\n"
            "      if not succ then return nil, msg end\n"
            "   end\n"
            "   -- Load the CA certificates\n"
            "   if cfg.cafile or cfg.capath then\n"
            "      succ, msg = context.locations(ctx, cfg.cafile, cfg.capath)\n"
            "      if not succ then return nil, msg end\n"
            "   end\n"
            "   -- Set the verification options\n"
            "   succ, msg = optexec(context.setverify, cfg.verify, ctx)\n"
            "   if not succ then return nil, msg end\n"
            "   -- Set SSL options\n"
            "   succ, msg = optexec(context.setoptions, cfg.options, ctx)\n"
            "   if not succ then return nil, msg end\n"
            "   -- Set the depth for certificate verification\n"
            "   if cfg.depth then\n"
            "      succ, msg = context.setdepth(ctx, cfg.depth)\n"
            "      if not succ then return nil, msg end\n"
            "   end\n"
            "   return ctx\n"
            "end\n"
            "\n"
            "--\n"
            "--\n"
            "--\n"
            "function wrap(sock, cfg)\n"
            "   local ctx, msg\n"
            "   if type(cfg) == \"table\" then\n"
            "      ctx, msg = newcontext(cfg)\n"
            "      if not ctx then return nil, msg end\n"
            "   else\n"
            "      ctx = cfg\n"
            "   end\n"
            "   local s, msg = core.create(ctx)\n"
            "   if s then\n"
            "      core.setfd(s, sock:getfd())\n"
            "      sock:setfd(core.invalidfd)\n"
            "      return s\n"
            "   end\n"
            "   return nil, msg\n"
            "end\n";

    MODULE_HTTPS =
            "----------------------------------------------------------------------------\n"
            "-- LuaSec 0.4.1\n"
            "-- Copyright (C) 2009-2011 PUC-Rio\n"
            "--\n"
            "-- Author: Pablo Musa\n"
            "-- Author: Tomas Guisasola\n"
            "---------------------------------------------------------------------------\n"
            "\n"
            "local socket = require(\"socket\")\n"
            "local ssl    = require(\"ssl\")\n"
            "local ltn12  = require(\"ltn12\")\n"
            "local http   = require(\"socket.http\")\n"
            "local url    = require(\"socket.url\")\n"
            "\n"
            "local table  = require(\"table\")\n"
            "local string = require(\"string\")\n"
            "\n"
            "local try          = socket.try\n"
            "local type         = type\n"
            "local pairs        = pairs\n"
            "local getmetatable = getmetatable\n"
            "\n"
            "module(\"ssl.https\")\n"
            "\n"
            "_VERSION   = \"0.4.1\"\n"
            "_COPYRIGHT = \"LuaSec 0.4.1 - Copyright (C) 2009-2011 PUC-Rio\"\n"
            "\n"
            "-- Default settings\n"
            "PORT = 443\n"
            "\n"
            "local cfg = {\n"
            "  protocol = \"tlsv1\",\n"
            "  options  = \"all\",\n"
            "  verify   = \"none\",\n"
            "}\n"
            "\n"
            "--------------------------------------------------------------------\n"
            "-- Auxiliar Functions\n"
            "--------------------------------------------------------------------\n"
            "\n"
            "-- Insert default HTTPS port.\n"
            "local function default_https_port(u)\n"
            "   return url.build(url.parse(u, {port = PORT}))\n"
            "end\n"
            "\n"
            "-- Convert an URL to a table according to Luasocket needs.\n"
            "local function urlstring_totable(url, body, result_table)\n"
            "   url = {\n"
            "      url = default_https_port(url),\n"
            "      method = body and \"POST\" or \"GET\",\n"
            "      sink = ltn12.sink.table(result_table)\n"
            "   }\n"
            "   if body then\n"
            "      url.source = ltn12.source.string(body)\n"
            "      url.headers = {\n"
            "         [\"content-length\"] = #body,\n"
            "         [\"content-type\"] = \"application/x-www-form-urlencoded\",\n"
            "      }\n"
            "   end\n"
            "   return url\n"
            "end\n"
            "\n"
            "-- Forward calls to the real connection object.\n"
            "local function reg(conn)\n"
            "   local mt = getmetatable(conn.sock).__index\n"
            "   for name, method in pairs(mt) do\n"
            "      if type(method) == \"function\" then\n"
            "         conn[name] = function (self, ...)\n"
            "                         return method(self.sock, ...)\n"
            "                      end\n"
            "      end\n"
            "   end\n"
            "end\n"
            "\n"
            "-- Return a function which performs the SSL/TLS connection.\n"
            "local function tcp(params)\n"
            "   params = params or {}\n"
            "   -- Default settings\n"
            "   for k, v in pairs(cfg) do\n"
            "      params[k] = params[k] or v\n"
            "   end\n"
            "   -- Force client mode\n"
            "   params.mode = \"client\"\n"
            "   -- 'create' function for LuaSocket\n"
            "   return function ()\n"
            "      local conn = {}\n"
            "      conn.sock = try(socket.tcp())\n"
            "      local st = getmetatable(conn.sock).__index.settimeout\n"
            "      function conn:settimeout(...)\n"
            "         return st(self.sock, ...)\n"
            "      end\n"
            "      -- Replace TCP's connection function\n"
            "      function conn:connect(host, port)\n"
            "         try(self.sock:connect(host, port))\n"
            "         self.sock = try(ssl.wrap(self.sock, params))\n"
            "         try(self.sock:dohandshake())\n"
            "         reg(self, getmetatable(self.sock))\n"
            "         return 1\n"
            "      end\n"
            "      return conn\n"
            "  end\n"
            "end\n"
            "\n"
            "--------------------------------------------------------------------\n"
            "-- Main Function\n"
            "--------------------------------------------------------------------\n"
            "\n"
            "-- Make a HTTP request over secure connection.  This function receives\n"
            "--  the same parameters of LuaSocket's HTTP module (except 'proxy' and\n"
            "--  'redirect') plus LuaSec parameters.\n"
            "--\n"
            "-- @param url mandatory (string or table)\n"
            "-- @param body optional (string)\n"
            "-- @return (string if url == string or 1), code, headers, status\n"
            "--\n"
            "function request(url, body)\n"
            "  local result_table = {}\n"
            "  local stringrequest = type(url) == \"string\"\n"
            "  if stringrequest then\n"
            "    url = urlstring_totable(url, body, result_table)\n"
            "  else\n"
            "    url.url = default_https_port(url.url)\n"
            "  end\n"
            "  if http.PROXY or url.proxy then\n"
            "    return nil, \"proxy not supported\"\n"
            "  elseif url.redirect then\n"
            "    return nil, \"redirect not supported\"\n"
            "  elseif url.create then\n"
            "    return nil, \"create function not permitted\"\n"
            "  end\n"
            "  -- New 'create' function to establish a secure connection\n"
            "  url.create = tcp(url)\n"
            "  local res, code, headers, status = http.request(url)\n"
            "  if res and stringrequest then\n"
            "    return table.concat(result_table), code, headers, status\n"
            "  end\n"
            "  return res, code, headers, status\n"
            "end\n";
}

}
}
#endif
