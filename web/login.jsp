<%@ page contentType="text/html; charset=iso-8859-1" language="java" %>
<%
String error=request.getParameter("error");
if(error==null || error=="null"){
 error="";
}
%>
<html>
<head>
<title>Login JSP</title>
<script>
    function trim(s) 
    {
        return s.replace( /^\s*/, "" ).replace( /\s*$/, "" );
    }

    function validate()
    {
        if(trim(document.frmLogin.sUserName.value)=="")
        {
          alert("Login empty");
          document.frmLogin.sUserName.focus();
          return false;
        }
        else if(trim(document.frmLogin.sPwd.value)=="")
        {
          alert("password empty");
          document.frmLogin.sPwd.focus();
          return false;
        }
    }
</script>
</head>

<body>
<%@include file="head.html"%>
<br/>
<br/>
<br/>
           
        <h2 align="center">Login details</h2>
        <form name="frmLogin" onSubmit="return validate();" action="checklogin.jsp" method="post">
        <table align="center">
            <tr>
                <td>Username</td>
                <td><input type="text" name="sUserName" /></td>
            </tr>
            <tr>
                <td>Password</td>
                <td><input type="password" name="sPwd" /></td>
                
            </tr>
            <tr>
                <td></td>
                <td><input type="submit" name="sSubmit" value="Submit" /><input type="reset" value="Clear" />
                </td>
            </tr>
            <tr>
                <td> </td>
                <td><div><%=error%></div></td>
            </tr>
        </table>
        </form>
</body>
</html>