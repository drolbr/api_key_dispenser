#ifndef POSTRGRES_WRAPPER
#define POSTRGRES_WRAPPER

#include <libpq-fe.h>

#include <stdexcept>


struct PostgreSQL_Connection
{
  struct Error : std::runtime_error
  {
    Error() : std::runtime_error("PQstatus did not report CONNECTION_OK") {}
  };

  PostgreSQL_Connection();
  ~PostgreSQL_Connection();

  PGresult* exec(const char* command) const;

private:
  PGconn* conn;
};


struct PostgreSQL_Result
{
  struct Error : std::runtime_error
  {
    Error(const std::string& error_code) : std::runtime_error("PQstatus returned " + error_code) {}
  };

  PostgreSQL_Result(const PostgreSQL_Connection& conn, const char* command);
  ~PostgreSQL_Result();

  int col_size() const;
  const char* col_name(int pos) const;
  int row_size() const;
  const char* at(int row, int col) const;

private:
  PGresult* res;
  mutable int col_size_;
  mutable bool col_size_set;
};


#endif
