<%@ page language="java" import="java.sql.*" errorPage="" %>
<%
    String uadmin=request.getParameter("sUserName");
    String adminp=request.getParameter("sPwd");
    String doctor_id=request.getParameter("sUserName");
    String doctorp=request.getParameter("sPwd");
    String patient_id=request.getParameter("sUserName");
    String patientp=request.getParameter("sPwd");
    String message="User login successfully ";
    Connection conn = null;
    Class.forName("com.mysql.jdbc.Driver").newInstance();
    conn = DriverManager.getConnection("jdbc:mysql://localhost:3306/mhealthdefault","mhealth","mhealth");
    
    ResultSet rsdoLogin = null;
    PreparedStatement psdoLogin=null;
    if (uadmin.toLowerCase().equals("admin") && adminp.toLowerCase().equals("admin"))
            {
                response.sendRedirect("admin.jsp");
            }
                   
    try{
    String sqlOption="SELECT * FROM doctors where"
                    +" doctor_id=? and doctorp=?";
    
    psdoLogin=conn.prepareStatement(sqlOption);
    psdoLogin.setString(1,doctor_id);
    psdoLogin.setString(2,doctorp);
    
    rsdoLogin=psdoLogin.executeQuery();
    
    if(rsdoLogin.next())
    {
      //String sUserName=rsdoLogin.getString("d_name")+" "+rsdoLogin.getString("specialization");  
      String sUserName=rsdoLogin.getString("d_name");
      session.setAttribute("doctor_id",rsdoLogin.getString("doctor_id"));
      session.setAttribute("d_name",rsdoLogin.getString("d_name"));
      session.setAttribute("sUserName",sUserName);
      //response.sendRedirect("success.jsp?error="+message);
      response.sendRedirect("index.jsp?error="+message);
      
    }
    else
    {
      message="No Doctor Login found" ;
     // response.sendRedirect("login.jsp?error="+message);
    }
    }//try
    catch(Exception e)
    {
        e.printStackTrace();
    }//catch
   
       
      try{
    String sqlOption="SELECT * FROM patients where"
                    +" patient_id=? and patientp=?";
    
    psdoLogin=conn.prepareStatement(sqlOption);
    psdoLogin.setString(1,patient_id);
    psdoLogin.setString(2,patientp);
    
    rsdoLogin=psdoLogin.executeQuery();
    
    if(rsdoLogin.next())
    {
      //String sUserName=rsdoLogin.getString("d_name")+" "+rsdoLogin.getString("specialization");  
      String sUserName=rsdoLogin.getString("p_name");
      session.setAttribute("patient_id",rsdoLogin.getString("patient_id"));
      session.setAttribute("p_name",rsdoLogin.getString("p_name"));
      session.setAttribute("sUserName",sUserName);
      //response.sendRedirect("success.jsp?error="+message);
      response.sendRedirect("patientindex.jsp");
      
    }
    else
    {
      message="No Patient Login found" ;
      response.sendRedirect("login.jsp?error="+message);
    }
    }
    catch(Exception e)
    {
        e.printStackTrace();
    }
   
       finally{
       conn.close();
       
       }
    
    /// close object and connection
    
%>