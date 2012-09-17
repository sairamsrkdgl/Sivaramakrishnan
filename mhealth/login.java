package mhealth;
import java.io.*;
import javax.servlet.*;
import javax.servlet.http.*;
import java.sql.*;
 public class login extends HttpServlet{ 
    @Override
 public void doPost(HttpServletRequest request, HttpServletResponse response) 
         throws ServletException,IOException{
  response.setContentType("text/html");
  PrintWriter out = response.getWriter();
  Connection conn = null;
  //Class.forName("com.mysql.jdbc.Driver");
  //Connection con=DriverManager.getConnection("jdbc:mysql://localhost:3306/mhealthdefault","mhealth","mhealth");
  String url = "jdbc:mysql://localhost:3306/";
  String dbName = "mhealthdefault";
  String driver = "com.mysql.jdbc.Driver";
  String userName = "mhealth"; 
  String password = "mhealth";
  String username="";
  String userpass="";
  String strQuery= ""; 
  Statement st=null;
  ResultSet rs=null;
  HttpSession session = request.getSession(true);
  try {
	Class.forName(driver).newInstance();
	conn = DriverManager.getConnection(url+dbName,userName,password);
	if(request.getParameter("username")!=null && request.getParameter("password")!=null )
		{
                    username = request.getParameter("username").toString();
		    userpass = request.getParameter("password").toString();
                    strQuery="select * from doctors where doctor_id='"+username+"' and doctorp='"+userpass+"'";
					System.out.println(strQuery);
					st = conn.createStatement();
					rs = st.executeQuery(strQuery);
					int count=0;
					while(rs.next())
					{
					  
						session.setAttribute("username",rs.getString(2));
						count++;
					}

                    if(count>0)
					{
						response.sendRedirect("index.jsp");
					}
					else
					{
                                            response.sendRedirect("login.jsp");
					}
				  
				}
				else
				{
					 response.sendRedirect("login.jsp");
				}

				System.out.println("Connected to the database");
				 
				
				conn.close();
				System.out.println("Disconnected from database");
			} catch (Exception ex) {
			System.out.println(ex.getMessage());
                        }
  }
 }