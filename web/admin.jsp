<%-- 
    Document   : admin
    Created on : May 2, 2012, 4:54:19 PM
    Author     : rkumar
--%>

<%@page contentType="text/html" pageEncoding="windows-1252"%>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>Admin Page</title>
    </head>
    <body>
        <h2>Welcome to Admin home page</h2>
     <%@include file="head.html"%>
    <a href="newpatientform.jsp">New Patient</a><br/>
    <a href="newdoctorform.jsp">New Doctor </a><br/>
    <a href="newpatientdependentform.jsp">New Patient Dependent</a><br/>
    <a href="viewrecomendations.jsp">View Recommendations</a>  
    &nbsp;&nbsp;
    <%@include file="footer1.html"%>
    </body>
</html>
