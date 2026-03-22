
## Conceitos básicos

- .schema define tabelas, colunas, tipos, chaves primárias, chaves estrangeiras, índices, constraints, views e triggers.
- .tables tem os nomes das tabelas

## Nível básico

- SELECT
- WHERE
- INSERT
- UPDATE
- DELETE
- JOIN
- GROUP BY
- HAVING
- ORDER BY
- LIMIT
- Subqueries
- EXISTS

### Criar tabela

- CREATE TABLE usuarios ( id INTEGER, nome TEXT NOT NULL, email TEXT UNIQUE, ativo INTEGER DEFAULT 1 );

- PRIMARY KEY: garante que cada linha vai ter ID único.

OBS: no SLITE não é possível atribuir primary key depois que a tabela está criada.

### Atribuir chave primária a uma coluna

- ALTER TABLE users ADD PRIMARY KEY (id);

### Copiar dado de outra tabela

- INSERT INTO users_new (id, name) SELECT id, name FROM users;

### Apagar tabela

- DROP TABLE users;

### Atualizar tabelas

- UPDATE users SET nome = 'Denis Moraes' WHERE email = 'denisguimaraes2012@gmail.com';

### Renomear tabela

- ALTER TABLE users_new RANAME TO users;

### Alterar o tipo de uma coluna

- ALTER TABLE users ALTER COLUMN ativo BOOL;

OBS: no SQLITE não é possível mudar o tipo de uma coluna, é necessário recriar a tabela. 

### Adicionar item na tabela

- INSERT INTO users (nome, email, ativo) VALUES ('Denis', 'denisguimaraes2012@gmail.com', 1);

### Listar itens

- Listar todas as colunas: SELECT * FROM users;
- Listar todas as colunas com limite: SELECT * FROM users LIMIT 100;
- Listar todas as colunas com limite com offset: SELECT * FROM users LIMIT 100 OFFSET 20;
- Listar apenas algumas colunas: SELECT nome, email FROM users;
- Listar com filtro: SELECT * FROM users WHERE ativo = 1;
- Listar com mais de um filtro: SELECT * FROM users WHERE ativo = 1 AND nome = 'Denis';
- Listar de forma iterativa: SELECT * FROM users WHERE id IN (1, 2,3);
- Listar de com intervalo: SELECT * FROM users WHERE idade BETWEEN 18 AND 30;
- Listar com informação incompleta: SELECT * FROM users WHERE nome LIKE 'Den%';
- Listar item não preenchido: SELECT * FROM users WHERE email IS NULL;
- Listar com ordenação: SELECT * FROM users ORDER BY nome ASC;
- Calcular quantidade de itens: SELECT COUNT(*) FROM users;
- Calcular soma de itens: SELECT SUM(users.valor) FROM users;
- Calcular média dos itens de uma coluna: SELECT AVG(idade) FROM users;
- Contar por agrupamento: SELECT ativo, COUNT(*) FROM users GROUP BY ativo;
- Contar por condição: SELECT ativo, COUNT(*) FROM users GROUP BY ativo HAVING COUNT(*) > 5;


- Aliase: SELECT nome AS user_name FROM users;
- Remove itens duplicados: SELECT DISTINCT nome FROM users;
- Condicional: SELECT nome, CASE WHEN ativo = 1 THEN 'Ativo' ELSE 'Inativo' END AS status FROM users;
- Existe: SELECT EXISTS (SELECT 1 FROM users WHERE ativo = 0);

### Relacionamento entre tabelas

- JOIN: SELECT users.nome, pedidos.valor FROM users JOIN pedidos ON users.id = pedidos.user_id;
- LEFT JOIN: SELECT users.nome, pedidos.valor FROM users LEFT JOIN pedidos ON users.id = pedidos.user_id;
- JOIN + GROUP BY: SELECT users.nome, COUNT(pedidos.id) AS total_pedidos FROM users LEFT JOIN pedidos ON users.id = pedidos.user_id GROUP BY users.id;

### Obter primeiro registro da tabela

- SELECT * FROM usuarios ORDER BY id ASC LIMIT 1;

### Obter ultimo registro da tabela

- SELECT * FROM usuarios ORDER BY id DESC LIMIT 1;

### Subqueries

- WHERE: SELECT * FROM usuarios WHERE id = ( SELECT user_id FROM pedidos ORDER BY valor DESC LIMIT 1 );
- IN: SELECT * FROM usuarios WHERE id IN ( SELECT user_id FROM pedidos );
- NOT EXISTS: SELECT * FROM usuarios u WHERE NOT EXISTS ( SELECT 1 FROM pedidos p WHERE p.user_id = u.id );
- SELECT: SELECT u.nome, ( SELECT SUM(p.valor) FROM pedidos p WHERE p.user_id = u.id ) AS total_gasto FROM usuarios u;
- FROM: SELECT * FROM ( SELECT user_id, SUM(valor) AS total FROM pedidos GROUP BY user_id ) AS resumo WHERE total > 2000;

## Nível intermediário

- INDEX
- EXPLAIN QUERY PLAN
- CONSTRAINTS
- TRANSACTIONS
- UPSERT

## Nível avançado

- Modelagem realcional
- Common Table Expression
- Window Functions
- Triggers
- Otimização
- Migrations


# Exercícios

## Nível básico

- Listar todos os pedidos com o nome do usuário. Mostrar id do pedido, nome do usuário, valor, data. 
    SELECT p.id, u.nome, p.valor, p.data FROM usuarios u JOIN pedidos p ON u.id = p.user_id;

- Listar apenas pedidos de usuários ativos (ativo = 1).
    SELECT * FROM pedidos p JOIN usuarios u ON u.id = p.user_id WHERE u.ativo = 1;

- Mostrar todos os produtos comprados em cada pedido. Exibir id do produto, nome do produto, quantidade.
    SELECT pp.pedido_id, p.nome, pp.quantidade FROM produtos AS p JOIN pedidos_produtos AS pp ON p.id = pp.produto_id; 

- Calcular o total gasto por cada usuário. Exibir nome do usuário, soma total de pedidos, ordenar do maior para o menor;
    SELECT u.nome, SUM(p.valor) AS total_gasto FROM usuarios AS u JOIN pedidos AS p ON u.id = p.user_id GROUP BY u.nome ORDER BY total_gasto DESC;

- Mostrar usuários que já gastaram mais de 2000;
    SELECT u.nome FROM usuarios AS u JOIN pedidos AS p ON u.id = p.user_id GROUP BY u.id HAVING SUM(p.valor) > 
    2000;

- Mostrar quantos pedidos cada usuário fez; mostrar os que fizeram zero pedidos;
    SELECT u.nome, COUNT(p.id) AS total_pedidos FROM usuarios AS u LEFT JOIN pedidos AS p ON p.user_id = u.id GROUP BY u.id ORDER BY total_pedidos DESC;

- Listar usuários que nunca fizeram pedido;
    SELECT u.nome as "Nome" FROM usuarios AS u WHERE NOT EXISTS ( SELECT 1 FROM pedidos AS p WHERE p.user_id = u.id );

- Mostrar o pedido com maior valor;
    SELECT p.id AS "Item", p.valor AS "Maior Valor" FROM pedidos AS p WHERE p.valor = (SELECT MAX(valor) FROM pedidos);

- Mostrar o produto mais vendido (em quantidade total);
    

- Calcular o valor total real de cada pedido (quantidade * preço);

- Verificar se existe inconsistência;

- Ranking de produtos por faturamento;

- Top 3 usuários que mais gastam;

- Produto que aparece no maior número de pedidos diferentes;

- Ticket médio por usuário;