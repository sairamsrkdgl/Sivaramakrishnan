<%@page contentType="text/html" pageEncoding="windows-1252"%>
<%@page import="mhealth.*,java.util.*,java.lang.String"%>
<%@page contentType="text/html" import="java.util.*,mhealth.*" %>
<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
        <title>Doctor Recommendations</title>
    </head>
    <body>
         <%@include file="head.html"%>
       
 <%  //  String patientID = request.getParameter("pid");
     // String pid = (String) session.getAttribute("patient_id");
     
    //  String value = par.getPname();
    //  System.out.println(value);    
      //String pname = request.getParameter("pname_f");
     // do only when parameters are present
    // if ( pname != null ) {
   //         String fromage = request.getParameter("fromage");
   //       String toage = request.getParameter("toage");
       //  String gender = request.getParameter("gender");
        
         ArrayList<Patient> patients = Patient.getRecommendations();
         
         if ( patients.size() == 0 )
             out.println("<h3>Sorry! No Recommendations are assigned!</h3>");
         else {
                     
             %>
       <br/>
       <h3>Recommendations from Doctor(s)</h3>
       <div align="center"><table border=1 width="100%">
          <tr>
          <th>Refer to Doctor</th>
          <th>Recommendation from Doctor</th>
          <th>Patient Information</th>
          <th>Accept</th>
     
         
          </tr>
       <%
         for ( Patient p : patients) {
             
             String assdocname = p.getDescription();
             String assbydoc = p.getDaddress();
             String patientdetails = p.getBtemp();
         
             Patient part = Patient.getRecDetails(assdocname);
             String idr = part.getPatientp();
            // session.setAttribute("idr", idr);
             String idr1 = part.getBgroup();
             String idr2 = part.getHrate();
             
             Patient part1 = Patient.getRecDetails1(assbydoc);  
             String idrr = part1.getPatientp();
             String idrr1 = part1.getBgroup();
             String idrr2 = part1.getHrate();
             
             Patient part2 = Patient.getRecDetails2(patientdetails); 
             String idrrr = part2.getPatientp(); 
             String idrrr1 = part2.getBgroup(); 
           //  session.setAttribute("idrrr", idrrr);
            
             String rid = p.getBpressure();
             
               out.println( "<tr>");
          //     out.println("<td>" + p.getBpressure() + "</td>");
               out.println("<td>"+ idr +", "+idr1+", "+idr2+"</td>");
               out.println("<td>"+ idrr +", "+idrr1+", "+idrr2+"</td>");                         
               out.println("<td>"+ idrrr +", "+idrrr1+"</td>");  
              
                        
          //     if((par.getPname()=="y")&(par.getPname()!="n"))               
        //      {  
          // System.out.println("The value  ------------------------"+par.getPname());   
             // out.println("<td>Enable</td>"); 
              out.println("<td><a href=accept.jsp?pid=" + rid +">Enable</a>" + "</td>");
             // out.println("<td><a href=reject.jsp?pid=" + rid +">Disable</a>" + "</td>");
          //    }else{
                  
                 
             // out.println("<td>Disable</td>");
              
         //     out.println("<td>Disable</td>");
           //  }
            // if(value.equalsIgnoreCase("y"))
           //  {
          //    out.println("<td>Enable</td>");
           //   out.println("<td><a href=accept.jsp>Accept</a>" + "</td>");
          //   }else{                                        
           //   out.println("<td><a href=reject.jsp>Reject</a>" + "</td>");
          //    out.println("<td>Disable</td>");  
            //      }
              out.println("</tr>");
         } //for
         out.println("</table>");
       }// else
      
    %>
       </div> <br>
       <a href="admin.jsp">Back</a>
    </body>
</html>
