<?php
include ("banco.php");

$result = queryDb("SELECT format(temperatura,2) AS temperatura, format(humidade,2) AS humidade, ip, estacao, date_format(`data`,'%d/%m/%Y %H:%i') as data_envio FROM dados ORDER BY data DESC LIMIT 1");
$DADOS['temperatura'] = $result[0]['temperatura'];
$DADOS['humidade'] = $result[0]['humidade'];
$DADOS['ip'] = $result[0]['ip'];
$DADOS['estacao'] = $result[0]['estacao'];
$DADOS['data'] = $result[0]['data_envio'];
echo json_encode($DADOS);