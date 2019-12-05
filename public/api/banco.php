<?php
//VARIAVEIS
$host = "172.17.0.1";
$user = "fabio";
$pass = "123";
$db = "temp";

date_default_timezone_set('America/Sao_Paulo');
//CONEXAO COM O BANCO
$con = mysqli_connect($host, $user, $pass, $db);
mysqli_query($con, "SET NAMES 'utf8'");
mysqli_query($con, 'SET character_set_connection=utf8');
mysqli_query($con, 'SET character_set_client=utf8');
mysqli_query($con, 'SET character_set_results=utf8');
//FUNCAO INSERIR BANCO
function insertDb($info,$table){
    global $con;
    $count = 1;
    $fields = "";
    $values = "";
    foreach($info as $key=>$value){
        $fields .= $key; 
        $values .= "'".$value."'";
        if($count < sizeof($info)){
            $fields .= ",";
            $values .= ",";
        }
        $count++;
    }
    mysqli_query($con, "INSERT INTO $table($fields) VALUES($values)");
    return (mysqli_affected_rows($con) == 1) ? true : false;
}
//FUNCAO ATUALIZAR BANCO
function updateDb($info,$table,$field,$id) {
    global $con;
    $fields = "";
    $values = "";
    $count = 1;
    $sql = "UPDATE ".$table." SET ";
    foreach($info as $key=>$value){
        $fields = $key;
        $values = "'".$value."'";
        if($count < sizeof($info)){
            $values .= ",";
        }
        $sql .= $fields . " = " . $values;
        $count++;
    }
    $sql .= " WHERE ". $field ." = '". $id."'";
    mysqli_query($con, $sql);
    // echo $sql;
    return (mysqli_affected_rows($con) == 1) ? true : false;
}
//FUNCAO DELETAR BANCO
function deleteDb($table,$field,$id) {
    global $con;
    mysqli_query($con, "DELETE FROM $table WHERE $field = '$id'");
    return (mysqli_affected_rows($con) == 1) ? true : false;
}
//FUNCAO PESQUISAR BANCO
function queryDb($query){
    global $con;
    $rs = mysqli_query($con, $query);
    $i=0;
    $ret = array();
    while ($row = mysqli_fetch_assoc($rs)) {
        foreach ($row as $key => $value) {
            $ret[$i][$key] = $value;
        }
        $i++;
    }
    return ($ret);
}
//FUNCAO CONTAR BANCO
function countDb($query){
    global $con;
    $rs = mysqli_query($con, $query);
    return  mysqli_num_rows($rs);
}
function cloneDb($tabela_ori,$tabela_dst) {
    global $con;
    $rs = mysqli_query($con, "CREATE TABLE $tabela_dst SELECT * FROM $tabela_ori");
    return (mysqli_affected_rows($con) == 1) ? true : false;
}
function clearDb($tabela) {
    global $con;
    $rs = mysqli_query($con, "truncate table $tabela");
    return (mysqli_affected_rows($con) == 1) ? true : false;
}
?>