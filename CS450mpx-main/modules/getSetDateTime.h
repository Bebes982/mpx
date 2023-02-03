/*
  Procedure..: getDate
  Description..: Prints the date held by the real time clock yyyy-mm-dd
*/
void getDate();

/*
  Procedure..: getTime
  Description..: Prints the time held by the real time clock in the form hh:mm:ss
*/
void getTime();

/*
  Procedure..: setDate
  Description..: Prints the date held by the real time clock in the form yyyy-mm-ss.
                 Does not check for invalid input
  Params..: char* str - string containg date in the form "yyyy-mm-ss"
*/
void setDate(char* str);

/*
  Procedure..: setTime
  Description..: Prints the time held by the real time clock in the form hh:mm:ss.
                 Does not check for invalid input
  Params..: char* str - string containg time in the form "hh:mm:ss"
*/
void setTime(char* str);

unsigned char int_to_bcd(int num);
int bcd_to_int(char bcd);
