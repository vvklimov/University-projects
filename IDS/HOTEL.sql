DROP TABLE ZAKAZNICI CASCADE CONSTRAINTS;
DROP TABLE ZAMESTNANCE CASCADE CONSTRAINTS;
DROP TABLE PLATBY CASCADE CONSTRAINTS;
DROP TABLE POKOJI CASCADE CONSTRAINTS;
DROP TABLE SLUZBY CASCADE CONSTRAINTS;
DROP TABLE POBYTY CASCADE CONSTRAINTS;
DROP TABLE zam_sluzby CASCADE CONSTRAINTS;
DROP TABLE pok_sluzby CASCADE CONSTRAINTS;
DROP TABLE pob_sluzby CASCADE CONSTRAINTS;
DROP TABLE pob_pok CASCADE CONSTRAINTS;
DROP TABLE STATICKE_SLUZBY CASCADE CONSTRAINTS;
DROP TABLE DYNAMICKE_SLUZBY CASCADE CONSTRAINTS;

DROP SEQUENCE zakaznik_seq;
DROP SEQUENCE zamestnanec_seq;
DROP SEQUENCE platba_seq;
DROP SEQUENCE pokoji_seq;
DROP SEQUENCE sluzby_seq;
DROP SEQUENCE pobyty_seq;

CREATE TABLE ZAKAZNICI (
cislo_zak int NOT NULL CONSTRAINT pk_zak PRIMARY KEY,
jmeno varchar(30) NOT NULL,
prijmeni varchar(30) NOT NULL,
tel_cislo number(12) NOT NULL,
pas int NOT NULL

);

CREATE TABLE ZAMESTNANCE (
id_zam number(6) NOT NULL CONSTRAINT pk_zam PRIMARY KEY,
jmeno varchar(30) NOT NULL,
prijmeni varchar(30) NOT NULL,
tel_cislo number(12) NOT NULL,
pozice varchar(50),
pas int NOT NULL
);

CREATE TABLE PLATBY (
cislo_platby number(10) NOT NULL CONSTRAINT pk_plat PRIMARY KEY,
castka number(6) NOT NULL,
datum date NOT NULL,
provedl INT NOT NULL,
dostal INT NOT NULL,
CONSTRAINT fk_provedl FOREIGN KEY (provedl) REFERENCES ZAKAZNICI,
CONSTRAINT fk_dostal FOREIGN KEY (dostal) REFERENCES ZAMESTNANCE
);

CREATE TABLE POKOJI(
cislo_pokoji number(3) NOT NULL CONSTRAINT pk_pok PRIMARY KEY,
stav varchar(15) NOT NULL,  
cena decimal(15,3) NOT NULL,
popis varchar(200) NOT NULL
);

CREATE TABLE POBYTY(
id_pobytu number(15) NOT NULL CONSTRAINT pk_pob PRIMARY KEY,
datum_check_in date,
datum_check_out date,
datum_zacatku date NOT NULL, 
datum_konci date NOT NULL,
cena decimal(16,3),
vyuziva int NOT NULL,
CONSTRAINT valid_check CHECK (datum_check_in < datum_check_out),
CONSTRAINT valid_datum CHECK (datum_zacatku <= datum_konci),
CONSTRAINT fk_vyuziva FOREIGN KEY (vyuziva) REFERENCES ZAKAZNICI
);

CREATE TABLE SLUZBY(
id_sluz number PRIMARY KEY,
stav varchar(15),
cena decimal(15,3) NOT NULL,
nazev varchar(100)
);

--tabulky pro vazbu many-to-many
CREATE TABLE zam_sluzby (
id_zam number(6),
id_sluz number,
CONSTRAINT fk_zam FOREIGN KEY (id_zam) REFERENCES ZAMESTNANCE,
CONSTRAINT fk_sluz_a FOREIGN KEY (id_sluz) REFERENCES SLUZBY,
CONSTRAINT pk_zam_sl PRIMARY KEY (id_zam, id_sluz)
);

CREATE TABLE pok_sluzby (
cislo_pokoji number(3),
id_sluz number,
CONSTRAINT fk_pok_a FOREIGN KEY (cislo_pokoji) REFERENCES POKOJI,
CONSTRAINT fk_sluz_b FOREIGN KEY (id_sluz) REFERENCES SLUZBY,
CONSTRAINT pk_pok_sl PRIMARY KEY (cislo_pokoji, id_sluz)
);

CREATE TABLE pob_sluzby (
id_pobytu number(15),
id_sluz number,
CONSTRAINT fk_pob_a FOREIGN KEY (id_pobytu) REFERENCES POBYTY,
CONSTRAINT fk_sluz_c FOREIGN KEY (id_sluz) REFERENCES SLUZBY,
CONSTRAINT pk_pob_sl PRIMARY KEY (id_pobytu, id_sluz)
);

CREATE TABLE pob_pok (
cislo_pokoji number(3),
id_pobytu number(15),
CONSTRAINT fk_pob_b FOREIGN KEY (id_pobytu) REFERENCES POBYTY,
CONSTRAINT fk_pok_b FOREIGN KEY (cislo_pokoji) REFERENCES POKOJI,
CONSTRAINT pk_pob_pok PRIMARY KEY (id_pobytu, cislo_pokoji)
);

/*v tabulkach pro podtypy STATICKE_SLUZBY a DYNAMICKE_SLUZBY
pouzit primarni klic id_sluz nadtypu SLUZBY*/
CREATE TABLE STATICKE_SLUZBY (
id_sluz number NOT NULL,
technicky_popis varchar(100),
CONSTRAINT fk_stat FOREIGN KEY (id_sluz) REFERENCES SLUZBY (id_sluz),
CONSTRAINT pk_stat PRIMARY KEY (id_sluz)
);

CREATE TABLE DYNAMICKE_SLUZBY (
id_sluz number NOT NULL,
popis_prace varchar(100),
CONSTRAINT fk_din FOREIGN KEY (id_sluz) REFERENCES SLUZBY (id_sluz),
CONSTRAINT pk_din PRIMARY KEY (id_sluz)
);

CREATE SEQUENCE zakaznik_seq
  START WITH 1
  INCREMENT BY 1;

CREATE SEQUENCE zamestnanec_seq
  START WITH 1
  INCREMENT BY 1;
  
CREATE SEQUENCE platba_seq
  START WITH 1
  INCREMENT BY 1;

CREATE SEQUENCE pokoji_seq
  START WITH 1
  INCREMENT BY 1;

CREATE SEQUENCE pobyty_seq
  START WITH 1
  INCREMENT BY 1; 

CREATE SEQUENCE sluzby_seq
  START WITH 1
  INCREMENT BY 1;

/*###########################
  TRIGGERY
############################*/
  
--kontroluje jestli pas uz existue
CREATE OR REPLACE TRIGGER check_pas_insert
BEFORE INSERT ON ZAKAZNICI
FOR EACH ROW
DECLARE
  cnt NUMBER;
BEGIN
  SELECT COUNT(*) INTO cnt
  FROM ZAKAZNICI
  WHERE pas = :NEW.pas;
  
  IF cnt > 0 THEN
    RAISE_APPLICATION_ERROR(-20001, 'Tento pas je uz pouzivan');
  END IF;
END;
/

--pokud zakaznik bydlel trikrat v hotelu tento rok, dostava slevu 10%
CREATE OR REPLACE TRIGGER discount
BEFORE INSERT OR UPDATE ON POBYTY
FOR EACH ROW
DECLARE
    cnt NUMBER;
BEGIN
    SELECT COUNT(*) INTO cnt
    FROM POBYTY
    WHERE vyuziva = :NEW.vyuziva
    AND EXTRACT(YEAR FROM datum_zacatku) = EXTRACT(YEAR FROM SYSDATE);

    IF cnt > 3 THEN
        :NEW.cena := :NEW.cena * 0.9;
    END IF;
END;
/

/*###########################
  INSERT
############################*/

INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, 'Jan', 'Novak', 420123456789, 123123123);
INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, 'Petr', 'Pavel', 420133830334, 101918171);
INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, 'Jaroslav', 'Dytrych', 420000111222, 757675767);
INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, 'Jozef', 'Fifo', 420333750586, 787898666);
INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, 'Tomas', 'Lifo', 420533645816, 201983279);

INSERT INTO ZAMESTNANCE (id_zam, jmeno, prijmeni, tel_cislo, pozice, pas) VALUES (zamestnanec_seq.NEXTVAL, 'Honza', 'Cerny', 420608352860, 'recepcni', 802211922);
INSERT INTO ZAMESTNANCE (id_zam, jmeno, prijmeni, tel_cislo, pozice, pas) VALUES (zamestnanec_seq.NEXTVAL, 'Monika', 'Prochazka', 420262357770, 'recepcni', 833434322);
INSERT INTO ZAMESTNANCE (id_zam, jmeno, prijmeni, tel_cislo, pozice, pas) VALUES (zamestnanec_seq.NEXTVAL, 'Ashot', 'Karakisyan', 420852456951, 'ridic', 201474145);

INSERT INTO PLATBY (cislo_platby, castka, datum, provedl, dostal) VALUES (platba_seq.NEXTVAL, 6500, TO_DATE('2023-03-14', 'yyyy-mm-dd'), 4, 1 );
INSERT INTO PLATBY (cislo_platby, castka, datum, provedl, dostal) VALUES (platba_seq.NEXTVAL, 8500, TO_DATE('2023-03-16', 'yyyy-mm-dd'), 2, 2 );
INSERT INTO PLATBY (cislo_platby, castka, datum, provedl, dostal) VALUES (platba_seq.NEXTVAL, 15750, TO_DATE('2023-03-23', 'yyyy-mm-dd'), 5, 1 );

INSERT INTO POKOJI (cislo_pokoji, stav, cena, popis) VALUES (pokoji_seq.NEXTVAL, 'volny', 5500, 'standardni pokoj pro 3 osoby');
INSERT INTO POKOJI (cislo_pokoji, stav, cena, popis) VALUES (pokoji_seq.NEXTVAL, 'obsazen', 7500, 'pokoj pro rodinu z 4 osob');
INSERT INTO POKOJI (cislo_pokoji, stav, cena, popis) VALUES (pokoji_seq.NEXTVAL, 'obsazen', 4400, 'standardni pokoj pro 2 osoby');
INSERT INTO POKOJI (cislo_pokoji, stav, cena, popis) VALUES (pokoji_seq.NEXTVAL, 'k uklidu', 15750, 'luxusni penthouse pro 6 osob');

INSERT INTO POBYTY (id_pobytu, datum_check_in, datum_check_out, datum_zacatku, datum_konci, cena, vyuziva) VALUES (pobyty_seq.NEXTVAL, 	TO_DATE('2023-03-14 13:27:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-15 11:20:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-14', 'yyyy-mm-dd'), TO_DATE('2023-03-18', 'yyyy-mm-dd'), 6500, 4);
INSERT INTO POBYTY (id_pobytu, datum_check_in, datum_check_out, datum_zacatku, datum_konci, cena, vyuziva) VALUES (pobyty_seq.NEXTVAL, 	TO_DATE('2023-03-16 13:47:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-17 11:24:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-16', 'yyyy-mm-dd'), TO_DATE('2023-03-17', 'yyyy-mm-dd'), 8500, 2);
INSERT INTO POBYTY (id_pobytu, datum_check_in, datum_check_out, datum_zacatku, datum_konci, cena, vyuziva) VALUES (pobyty_seq.NEXTVAL, 	TO_DATE('2023-03-23 13:37:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-24 10:23:18', 'YYYY-MM-DD HH24:MI:SS'), TO_DATE('2023-03-23', 'yyyy-mm-dd'), TO_DATE('2023-03-24', 'yyyy-mm-dd'), 15750, 5);

INSERT INTO SLUZBY (id_sluz, stav, cena, nazev) VALUES (sluzby_seq.NEXTVAL, 'dostupna', 500, 'Zvirata');
INSERT INTO SLUZBY (id_sluz, stav, cena, nazev) VALUES (sluzby_seq.NEXTVAL, 'dostupna', 1000, 'Kyvadlova doprava');
INSERT INTO SLUZBY (id_sluz, stav, cena, nazev) VALUES (sluzby_seq.NEXTVAL, 'dostupna', 100, 'Sluzba buzeni/Budik');
INSERT INTO SLUZBY (id_sluz, stav, cena, nazev) VALUES (sluzby_seq.NEXTVAL, 'dostupna', 2000, 'Minibar');
INSERT INTO SLUZBY (id_sluz, stav, cena, nazev) VALUES (sluzby_seq.NEXTVAL, 'dostupna', 200, 'Snidane do pokoje');

INSERT INTO zam_sluzby (id_zam, id_sluz) VALUES (2, 3);
INSERT INTO zam_sluzby (id_zam, id_sluz) VALUES (1, 5);
INSERT INTO zam_sluzby (id_zam, id_sluz) VALUES (3, 2);

INSERT INTO pok_sluzby (cislo_pokoji, id_sluz) VALUES (4, 4);
INSERT INTO pok_sluzby (cislo_pokoji, id_sluz) VALUES (3, 4);

INSERT INTO pob_sluzby (id_pobytu, id_sluz) VALUES (1, 2); --Kyvadlova doprava
INSERT INTO pob_sluzby (id_pobytu, id_sluz) VALUES (2 ,2);

INSERT INTO pob_pok (cislo_pokoji, id_pobytu) VALUES (1, 1);
INSERT INTO pob_pok (cislo_pokoji, id_pobytu) VALUES (2, 2);
INSERT INTO pob_pok (cislo_pokoji, id_pobytu) VALUES (4, 3);


INSERT INTO STATICKE_SLUZBY (id_sluz, technicky_popis) VALUES (1, 'Muzete bydlet u nas se svym zviratkem');
INSERT INTO STATICKE_SLUZBY (id_sluz, technicky_popis) VALUES (4, 'Primo v pokoji bude na vas cekat minibar');

INSERT INTO DYNAMICKE_SLUZBY (id_sluz, popis_prace) VALUES (2, 'Dostavime vas do hotelu z divadla/letisti');
INSERT INTO DYNAMICKE_SLUZBY (id_sluz, popis_prace) VALUES (3, 'Probudime vas ve spravnou hodinu');
INSERT INTO DYNAMICKE_SLUZBY (id_sluz, popis_prace) VALUES (5, 'Nemusite chodit na snidani do nase restaurace, radi jsme dostavime vam do pokoji');

/*###########################
  SELECT DOTAZY
############################*/


-- vypise seznam plateb ktere prijal  zamestnanec (2 tabulky)
SELECT Z.jmeno, Z.prijmeni, P.*
  FROM ZAMESTNANCE Z, PLATBY P
  WHERE Z.id_zam = P.dostal AND Z.id_zam = 1;

--vypise seznam pokoji ve kterych je minibar (2 tabulky)
SELECT P.cislo_pokoji, P.popis
  FROM POKOJI P, pok_sluzby PS
  WHERE PS.id_sluz = 4 AND PS.cislo_pokoji = P.cislo_pokoji; -- 4 je unikantni id sluzby minibar

-- vypise seznam zamnestnance, ktere poskytuje nejake sluzby s jejich nazve (3 tabulky)
SELECT Z.id_zam, Z.jmeno, Z.prijmeni, S.nazev AS poskytuje
  FROM ZAMESTNANCE Z, zam_sluzby ZS, SLUZBY S
  WHERE Z.id_zam = ZS.id_zam AND ZS.id_sluz = S.id_sluz;

--vypise kolik pokoji nachazi v jakem stave (volny/obsazen/k uklidu)    (GROUP BY + agregacni funkce)
SELECT P.stav, COUNT (P.stav) AS POCET
  FROM POKOJI P
  GROUP BY P.stav
  ORDER BY COUNT(P.stav);

--vypise kolik transakce prijal kazdy zamestnanec od nejvetsiho poctu k nejmensimu
SELECT Z.prijmeni, COUNT(Z.prijmeni) AS PocetPrijatychPlateb
  FROM ZAMESTNANCE Z, PLATBY P
  WHERE P.dostal = Z.id_zam
  GROUP BY Z.prijmeni;

--vypise zakazniki ktere jeste zadny pobyt v hotelu nemeli
SELECT *
  FROM ZAKAZNICI Z
  WHERE NOT EXISTS (
    SELECT *
    FROM POBYTY
    WHERE POBYTY.vyuziva = Z.cislo_zak);

--vypise zakazniky kteri zustali v hotelu v roce 2023
SELECT Z.jmeno, Z.prijmeni, Z.tel_cislo
    FROM ZAKAZNICI Z
    WHERE Z.cislo_zak IN (
        SELECT P.vyuziva
        FROM POBYTY P
        WHERE P.datum_check_in >= '01-01-2023' AND P.datum_check_in < '01-01-2024');



/*###########################
 PROCEDURY
############################*/
-- vypocita mesicni income hotelu: hotels_monthly_income(mm, yyyy)
CREATE OR REPLACE PROCEDURE hotels_monthly_income(mm NUMBER, yyyy NUMBER) AS
    monthly_income NUMBER;
    trans_exist NUMBER;
BEGIN
    SELECT COUNT(*) INTO trans_exist
    FROM platby
    WHERE EXTRACT(MONTH FROM datum) = mm AND EXTRACT(YEAR FROM datum) = yyyy;
    IF TO_DATE(mm || '-' || yyyy, 'MM-YYYY') > SYSDATE THEN
        RAISE_APPLICATION_ERROR(-20001, 'Wrong date.');
    END IF;
    IF trans_exist = 0 THEN
        DBMS_OUTPUT.PUT_LINE('There are no transactions in this month.');
        RETURN;
    END IF;
    SELECT SUM(castka) INTO monthly_income
    FROM platby
    WHERE EXTRACT(MONTH FROM datum) = mm AND EXTRACT(YEAR FROM datum) = yyyy;
    
    DBMS_OUTPUT.PUT_LINE('Monthly income for ' || mm || '/' || yyyy || ': ' || monthly_income || ' CZK.');
EXCEPTION
    WHEN OTHERS THEN
        DBMS_OUTPUT.PUT_LINE('An error occurred: ' || SQLERRM);
END;
/
--procedura pro pridani novych zakazniku do systemu
CREATE OR REPLACE PROCEDURE pridat_zakaznika (
p_jmeno IN VARCHAR2,
p_prijmeni IN VARCHAR2,
p_tel_cislo IN NUMBER,
p_pas IN INT
) AS
v_count INTEGER;
BEGIN
SELECT COUNT(*) INTO v_count FROM ZAKAZNICI WHERE jmeno = p_jmeno AND prijmeni = p_prijmeni AND tel_cislo = p_tel_cislo AND pas = p_pas;
IF v_count = 0 THEN
INSERT INTO ZAKAZNICI (cislo_zak, jmeno, prijmeni, tel_cislo, pas) VALUES (zakaznik_seq.NEXTVAL, p_jmeno, p_prijmeni, p_tel_cislo, p_pas);
COMMIT;
DBMS_OUTPUT.PUT_LINE('Novy zakaznik byl pridan do systemu.');
ELSE
RAISE_APPLICATION_ERROR(-20001, 'Tento zakaznik uz existuje v systemu.');
END IF;
EXCEPTION
WHEN OTHERS THEN
ROLLBACK;
DBMS_OUTPUT.PUT_LINE('Chyba: ' || SQLERRM);
END pridat_zakaznika;
/
SET SERVEROUTPUT ON;
-- spusteni procedury s ruznymi parametry:
exec hotels_monthly_income(10, 2023);
exec hotels_monthly_income(3, 2023); 
exec hotels_monthly_income(4, 2023); 
exec pridat_zakaznika('Jan', 'Novak', 420123456789, 123123123);
exec pridat_zakaznika('Jan', 'Novak', 420123436789, 122123123);

/*###########################
  EXPLAIN PLAN A INDEX
############################*/
        
CREATE INDEX idx_cislo_zak ON POBYTY(vyuziva);

EXPLAIN PLAN FOR
SELECT ZAKAZNICI.jmeno, COUNT(*) as pocet_pobytu
FROM ZAKAZNICI
JOIN POBYTY ON ZAKAZNICI.cislo_zak = POBYTY.vyuziva
GROUP BY ZAKAZNICI.jmeno;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

/*-------------------------------------------------------------------------------------
| Id  | Operation           | Name          | Rows  | Bytes | Cost (%CPU)| Time     |
-------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT    |               |     3 |   129 |     4  (25)| 00:00:01 |
|   1 |  HASH GROUP BY      |               |     3 |   129 |     4  (25)| 00:00:01 |
|   2 |   NESTED LOOPS      |               |     3 |   129 |     3   (0)| 00:00:01 |
|   3 |    TABLE ACCESS FULL| ZAKAZNICI     |     5 |   150 |     3   (0)| 00:00:01 |
|*  4 |    INDEX RANGE SCAN | IDX_CISLO_ZAK |     1 |    13 |     0   (0)| 00:00:01 |
-------------------------------------------------------------------------------------*/
                                                        

DROP INDEX idx_cislo_zak;

EXPLAIN PLAN FOR
SELECT ZAKAZNICI.jmeno, COUNT(*) as pocet_pobytu
    FROM ZAKAZNICI
    JOIN POBYTY ON ZAKAZNICI.cislo_zak = POBYTY.vyuziva
        GROUP BY ZAKAZNICI.jmeno;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY);

/*-------------------------------------------------------------------------------------------
| Id  | Operation                     | Name      | Rows  | Bytes | Cost (%CPU)| Time     |
-------------------------------------------------------------------------------------------
|   0 | SELECT STATEMENT              |           |     3 |   168 |     6  (34)| 00:00:01 |
|   1 |  HASH GROUP BY                |           |     3 |   168 |     6  (34)| 00:00:01 |
|   2 |   NESTED LOOPS                |           |     3 |   168 |     5  (20)| 00:00:01 |
|   3 |    NESTED LOOPS               |           |     3 |   168 |     5  (20)| 00:00:01 |
|   4 |     VIEW                      | VW_GBF_7  |     3 |    78 |     4  (25)| 00:00:01 |
|   5 |      HASH GROUP BY            |           |     3 |    39 |     4  (25)| 00:00:01 |
-------------------------------------------------------------------------------------------*/

/*###########################
  PRISTUPOVA PRAVA
############################*/

GRANT ALL PRIVILEGES ON ZAKAZNICI TO xivano08;
GRANT ALL PRIVILEGES ON ZAMESTNANCE TO xivano08;
GRANT ALL PRIVILEGES ON POKOJI TO xivano08;
GRANT ALL PRIVILEGES ON SLUZBY TO xivano08;
GRANT ALL PRIVILEGES ON POBYTY TO xivano08;
GRANT ALL PRIVILEGES ON PLATBY TO xivano08;
GRANT ALL PRIVILEGES ON pob_pok TO xivano08;
GRANT ALL PRIVILEGES ON pok_sluzby TO xivano08;
GRANT ALL PRIVILEGES ON pob_sluzby TO xivano08;
GRANT ALL PRIVILEGES ON DYNAMICKE_SLUZBY TO xivano08;
GRANT ALL PRIVILEGES ON STATICKE_SLUZBY TO xivano08;

/*###########################
  MATERIALIZOVANY POHLED
############################*/

DROP MATERIALIZED VIEW mv_zakaznici_pobyty;
CREATE MATERIALIZED VIEW mv_zakaznici_pobyty
BUILD IMMEDIATE
REFRESH ON COMMIT
AS
SELECT z.jmeno, z.prijmeni, p.datum_zacatku, p.datum_konci
    FROM zakaznici z
    JOIN pobyty p ON z.cislo_zak = p.vyuziva;

SELECT * FROM mv_zakaznici_pobyty;

--vypise zamestnance a sumu prijatech plateb
WITH platby_sum AS (
  SELECT dostal, SUM(castka) AS suma_castek
  FROM platby
  GROUP BY dostal
)
SELECT z.jmeno, z.prijmeni,
       CASE 
         WHEN p.suma_castek IS NULL THEN 0 
         ELSE p.suma_castek 
       END AS celkova_castka
FROM zamestnance z
LEFT JOIN platby_sum p ON z.id_zam = p.dostal
ORDER BY z.prijmeni, z.jmeno;


