#include <libpq-fe.h>
#include "postgresql_wrapper.h"

#include <cstdlib>
#include <cstring>
#include <iostream>


PostgreSQL_Connection::PostgreSQL_Connection() : conn(0)
{
  conn = PQconnectdb("dbname=osm_apikey");
  if (PQstatus(conn) != CONNECTION_OK)
  {
    PQfinish(conn);
    conn = 0;
    throw Error();
  }
}


PostgreSQL_Connection::~PostgreSQL_Connection()
{
  if (conn)
    PQfinish(conn);
}


PGresult* PostgreSQL_Connection::exec(const char* command) const
{
  return PQexec(conn, command);
}


PostgreSQL_Result::PostgreSQL_Result(const PostgreSQL_Connection& conn, const char* command)
    : res(0), col_size_(0), col_size_set(false)
{
  res = conn.exec(command);
  if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK)
  {
    std::string error_code = PQresStatus(PQresultStatus(res));
    PQclear(res);
    res = 0;
    throw Error(error_code);
  }
}


PostgreSQL_Result::~PostgreSQL_Result()
{
  if (res)
    PQclear(res);
}


int PostgreSQL_Result::col_size() const
{
  if (!res)
    return 0;

  if (!col_size_set)
  {
    col_size_ = PQnfields(res);
    col_size_set = true;
  }
  return col_size_;
}


const char* PostgreSQL_Result::col_name(int pos) const
{
  if (!res)
    return "";
  return PQfname(res, pos);
}


int PostgreSQL_Result::row_size() const
{
  if (!res)
    return 0;
  return PQntuples(res);
}


const char* PostgreSQL_Result::at(int row, int col) const
{
  if (!res)
    return "";
  const char* cell = PQgetvalue(res, row, col);
  if (!cell)
    return "";
  return cell;
}
