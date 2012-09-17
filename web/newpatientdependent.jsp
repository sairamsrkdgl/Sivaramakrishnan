<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
   
<jsp:useBean class="mhealth.PatientDependent" scope="page" id="PatientDependent" />
<jsp:setProperty name="PatientDependent" property="*" />
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>New Patient Dependent</title>
    </head>
    <body>
    <%@include file="head.html"%>
    <h2>New Patient Dependent</h2>
        <h3> 
            <%
            String pdependent = PatientDependent.add();
            if ( pdependent == null)
            out.println("Sorry! Could not add new Patient Dependent!");
            else
            out.println("New Patient Dependent details are added successfully. ");
            %>
        </h3>
         <%@include file="footer1.html"%>
    </body>
</html>