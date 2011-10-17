--[[
-- Sistema de OO em Lua.
-- Tanto este sistema quanto as classes criadas por ele utilizam o sistema de
-- modulos do Lua 5.1.
--
-- Este sistema simples de OO n�o � implementado com uso de metatables, mas com
-- um conceito de hierarquia por c�pia que � usado tanto na deriva��o de
-- classes como na instancia��o de objetos e suas propriedades.
--
-- Por usar c�pia as classes n�o s�o din�micas: uma altera��o em tempo de
-- execu��o em um m�todo de uma super-classe n�o ser� percebido pelas suas
-- sub-classes, assim como uma altera��o em um m�todo de uma classe n�o ter�
-- efeito sobre suas inst�ncias j� criadas.
--
-- Essa aparente fraqueza � na verdade uma das motiva��es da cria��o deste
-- sistema como veremos a seguir. 
-- Tamb�m por usar c�pia a cria��o das classes e objetos � mais lenta, mas a
-- utiliza��o durante o ciclo de vida � mais r�pida, uma vez que os m�todos e
-- propriedades do objeto est�o em sua pr�pria tabela.
-- Mais importante que isso � que a hierarquia tamb�m funciona para as
-- propriedades (em tabela) das classes, caso uma classe possua a mesma
-- propriedade de sua super-classe, os valores da tabela da sub-classe ser�o
-- completados pelos da super-classe, o mesmo vale na instancia��o
-- objeto->classe onde o uso dessa caracter�stica � ainda mais frequente.
--
-- O uso de propriedades p�blicas � incentivado contra o uso de m�todos
-- set()/get(), a versatilidade das propriedades � o argumento para isso:
-- Caso seja necess�rio tomar a��es TEMP
-- Caso oo._REQ seja o valor de uma propriedade, esta ser� requerida no momento
-- da instancia��o dos objetos e sua aus�ncia acarretar� em erro.
--
-- Cria��o de classes:
-- <example>
-- module 'C1'
-- prop1 = { a=1, b=2 }
-- prop2 = 1
-- function method1 (self) ... end
-- return oo.class(_M, oo.Object)
-- </example>
-- Cria "C1" herdando de Object, com suas propriedades e m�todos.
-- Como pode ser observado no exemplo acima e ao contr�rio da maioria dos
-- sistemas OO em Lua ou em outra linguagem, a chamada � fun��o oo.class(), que
-- propriamente cria a classe, deve ser feita ap�s a implementa��o da mesma,
-- isto �, met�dos e propriedades devem ser declarados antes da cria��o da
-- classe.
-- Isso se faz necess�rio pois � nesse momento que a classe criada copia tudo o
-- que for necess�rio de sua super-classe, portanto j� deve conter seus m�todos
-- e propriedades.
--
-- Instancia��o de objetos:
-- <example>
-- o1 = C1{ prop1 = true }
-- </example>
-- "o1" ter� automaticamente prop2=1, method1(), al�m de todos os m�todos e
-- propriedades de oo.Object e assim por diante.
--
-- Construtor:
--
-- O construtor de uma classe pode ser extendido da seguinte forma:
-- <example>
-- function C1.new (cls, obj)
-- 	obj = C1._SUPER.new(cls, obj)  -- chama o construtor acima
-- 	self = obj                     -- cria um alias para usar aqui
-- 	...
-- 	return obj                     -- retorna o objeto criado
-- end
-- </example>
-- Campos Est�ticos:
--
-- A conven��o para m�todos e propriedades est�ticas � coloc�-los precedidos de '_', sendo que
-- as propriedades devem tamb�m estar em mai�sculas e os m�todos N�O devem usar ':' em sua
-- defini��o:
-- <example>
-- C1 = {
-- 	_PROP = nil
-- }
-- function C1._method () ... end
-- -- e n�o: function C1:_method() ... end
-- </example>
-- Os campos est�ticos das classes n�o s�o passados para suas inst�ncias (mas s�o para suas
-- sub-classes).
-- Para usar um campo est�tico, deve-se chamar explicitamente a classe:
-- <example>
-- o1 = C1()
-- C1._PROP = 10
-- -- e n�o: o1._PROP = 10
-- </example>
-- Reflex�o:
--
-- Todos as classes possuem os campos "_SUPER" e "_NAME" que s�o respectivamente um ponteiro
-- para a super-classe e o nome da classe.
-- Todos os objetos possuem o campo "class" que � um ponteiro para sua classe.
-- <example>
-- print( o1.class._SUPER._NAME )
-- --> 'Object'
-- </example>
-- O m�todo 'is()' est� dispon�vel para perguntar ao objeto ou classe se ela � descendente de
-- algu�m:
-- <example>
-- o1.class.is('Object')
-- --> para qualquer objeto sempre vai dar true
-- </example>
--]]

local _G = _G
local string = string

module 'oo'

-- token unico utilizado em complete() para identificar campos em t2 obrigat�rios em t1
_REQ = {}

--[[
-- Valores de t1 sao completados com os de t2.
-- required: testa valores _REQ (instancia��o de objetos com propriedade obrigat�ria)
-- Faz a hierarquia de classes e instancia��o de objetos.
-- Caso os dois existam, mantem-se v1.
-- Casso o v1 seja '__nil__', mesmo que exista v2, v1 se tornar� nil.
-- Caso ambos sejam tabelas, faz-se a recursao.
-- Example:
-- <example>
-- t1 = { v1 = false, v2 = {1,2,3},    v3 = 1 }
-- t2 = { v1 = 50,    v2 = {a = true}, v3 = {-1} }
-- complete(t1, t2, '^_')
-- t1 = { v1 = false, v2 = {1,2,3, a=true}, v3 = 1 }
--
-- -- Heran�a de Classes:
-- c2 � super de c1
-- c2 = { _cls1 = xxx, prop1 = xxx, prop2 = _REQ, meth1 = f1() }
-- c1 = { prop1 = yyy }
-- complete(c1, c2)
-- -->  { _cls1 = xxx, prop1 = yyy, prop2 = _REQ, meth1 = f1() }
--
-- -- Inst�ncia:
-- o1 � inst�ncia de c2
-- o1 = {}
-- complete(o1, c1, true)
-- -->  { prop1 = yyy, prop2 = _REQ, meth1 = f1() }
-- --> error: field prop2 is mandatory
-- </example>
--]]
function complete (t1, t2, required)
	_G.assert(_G.type(t1) == 'table')
	_G.assert(_G.type(t2) == 'table')
	for k, v2 in _G.pairs(t2)
	do
if string.sub(k, 1, 1) ~= '_' then  -- continue, campo ignorado
		local v1 = t1[k]
		if v1 == '__nil__' then
			t1[k] = nil
		elseif (_G.type(v1) == 'table') and v1.class then
			-- objeto e' opaco, nao completar

		elseif v2 == _REQ then
			if v1 == nil then
				_G.assert( not required, 'field '..k..' is mandatory' )
				t1[k] = _REQUIRED   -- T1 e' class, passa o _REQ adiante
			end

		elseif (_G.type(v2) == 'table') and (not v2.class) then
			if t1[k] == nil then t1[k] = {} end
			if _G.type(t1[k]) == 'table' then
                complete(t1[k], v2, required)
			end

		elseif v1 == nil then
			t1[k] = v2
		end
	end
end
	return t1
end

--[[
-- Cria uma nova classe retornando-a.
-- cls: tabela classe com suas propriedades e m�todos
-- super: super-classe (default: Object)
--]]
function class (cls, super)
       _G.assert( (_G.type(cls)=='table') and cls._NAME, 'missing field _NAME on class' )
       cls._SUPER = super or Object
	   complete(cls, cls._SUPER, false)   -- required so no momento de instanciar objetos
       _G.setmetatable( cls, { __call  = function(self,...) return self.new(self,_G.unpack(arg)) end } )
       return cls
end

--[[
-- Retorna se o objeto ou classe e' (ou herda) de name.
-- cls: classe ou objeto sendo testado
-- name: nome a ser testado
--]]
function is (cls, name)
	if not cls then return false end
	if cls.class then cls = cls.class end
	if cls._NAME == name then return true end
	return is(cls._SUPER, name)
end

Object = {
	  _SUPER = nil          -- setado em class()
	, _NAME  = "oo.Object"  -- recebido com cls, em class()
	, class  = nil          -- setado em new()
	, id     = 'unamed'
	, new    = function(cls, obj)
				obj = obj or {}
				obj.class = cls
				complete(obj, cls, true)
				obj.id = obj.id .. '_' .. cls._NAME .. '_' .. _G.tostring(obj)
				return obj
			end
}

--[[
require 'oo'

A = { rect={x=-1000} }
A = class('A', A)

B = { rect={xx=100} }
B = class('B', B, A)

print('O '..tostring(Object))
print('A '..tostring(A), 'A._SUPER '..tostring(A._SUPER))
print('B '..tostring(B), 'B._SUPER '..tostring(B._SUPER))

b = B()
pr(A)
pr(B)
pr(b)
print('b.class '..tostring(b.class), 'b.class._SUPER '..tostring(b.class._SUPER))
_G.assert(b.class._SUPER == A)
_G.assert(b.class._SUPER._SUPER == Object)
_G.assert(B:is'Object' and b:is'A')

bb = b()
--]]
