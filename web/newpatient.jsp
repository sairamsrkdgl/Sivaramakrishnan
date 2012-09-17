<%@page contentType="text/html"%>
<%@page pageEncoding="UTF-8"%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN"
   "http://www.w3.org/TR/html4/loose.dtd">
   
<jsp:useBean class="mhealth.Patient" scope="page" id="patient" />
<jsp:setProperty name="patient" property="*" />
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
        <title>New Patient</title>
    </head>
    <body>
    <%@include file="head.html"%>
    <h2>New Patient</h2>
        <h3> 
            <%
            String selecteddoctor = request.getParameter("doctoridpf");
            String patient_id = patient.add();
            String aid_id = patient.patientadd(selecteddoctor);
            if ( patient_id == null & aid_id == null)
            out.println("Sorry! Could not add new patient!");
            else
            out.println("New patient details are added successfully. Patient Id : " + patient_id);
            %>
        </h3>
        <%@include file="footer1.html"%>
    </body>
   </html>
