<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
   
<jsp:useBean class="mhealth.Doctor" scope="page" id="Doctor" />
<jsp:setProperty name="Doctor" property="*" />
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>New Doctor</title>
    </head>
    <body>
    <%@include file="head.html"%>
    <h2>New Doctor</h2>
        <h3> 
            <%
            String doctor_id = Doctor.add();
            if ( doctor_id == null)
            out.println("Sorry! Could not add new doctor!");
            else
            out.println("New doctor details are added successfully. Doctor Id : " + doctor_id);
            %>
        </h3>
        
         <%@include file="footer1.html"%>
    </body>
</html>
