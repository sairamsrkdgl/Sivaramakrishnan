<%@page import="mhealth.Hparameters"%>
<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<jsp:useBean class="mhealth.Hparameters" scope="page" id="Hparameters" />
<jsp:setProperty name="Hparameters" property="*" />
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>JSP Page</title>
    </head>
    <body>
     <%@include file="head.html"%>
    
        <h3> 
            <%
          
           String hpid =Hparameters.add1();
           if ( hpid == null)
            out.println("Sorry! Could not add patient health parameters! ");
            else
            out.println("Patient health parameters are added successfully.");
            %>
        </h3>
        
        <a href="patientindex.jsp">Back</a>
        
    </body>
</html>
