<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">

<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title></title>
    </head>
    <body>
        <%@include file="head.html"%>
        <%
        String pid = (String) session.getAttribute("patient_id");
        //String str="y";
       // mhealth.Patient p=new mhealth.
         String doctor_id=request.getParameter("pid");
         String str="n";
      //   boolean done = false;
         boolean done1 = false;
    //     done = Patient.update1(str,pid);
         done1 = Patient.update2(str,pid,doctor_id);        
         if (! done1 )
              out.println("<h3>Doctor is disabled to view your records!!</h3>");
           else
              out.println("<h3>Doctor is disabled to view your records!</h3>");
                                               
        %>
        </br>
        <a href="settings1.jsp">Back</a>
    </body>
</html>
