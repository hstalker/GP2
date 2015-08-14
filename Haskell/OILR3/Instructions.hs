module OILR3.Instructions where

type Tid = Int -- Trav id


data Dim = Equ Int | GtE Int deriving (Show, Eq)

-- TODO: this may not be adequate due to sort order of n-tuples, but it will do
-- for a first cut
instance Ord Dim where
    compare (Equ _) (GtE _) = GT
    compare (Equ x) (Equ y) = compare x y
    compare (GtE x) (GtE y) = compare x y

type Pred = (Dim, Dim, Dim, Dim)


data Instr a b = 
      OILR Int
    -- Trav stack management
    | DROT                  -- Drop Trav
    | CLRT                  -- Clear Trav stack
    -- Graph manipulation
    | ADN a                 -- Add Node without Trav
    | ADE b a a             -- Add Edge between Nodes
    | DEN a                 -- Delete Node with id
    | DEE b                 -- Delete Edge with id
    | RTN a                 -- Set root flag on node
    | URN a                 -- unset root flag on node

    | ANT                   -- Add Node and push Trav
    | AET                   -- Add Edge between top two Travs
    | DNT                   -- Delete Node in Trav
    | DET                   -- Delete Edge in Trav
    | DNE                   -- Delete Node and Edge in Trav
    -- Stack machine prims
    | LIT Int               -- push literal on data stack
    | ADD                   -- add top two values on ds
    | SUB                   -- subtract top of stack from next on stack
    | SHL                   -- shift NoS left by ToS bits
    -- Definition
    | DEF String
    | END
    -- Graph search
    -- | CRS a Pred            -- conditional reset of trav
    | LUN a Pred
    | LUE b a a
    | XIE b a               -- extend match back along an incoming edge
    | XOE b a               -- extend match along an outgoing edge
    | NEC a a               -- no-edge condition
    -- flow control
    | CAL String | ALP String -- call rule or proc once or as-long-as-possible
    | RET                   -- unconditinoal return from current rule or proc
    | ORB a                 -- back to a if success flag is unset
    | ORF                   -- exit procedure if success flag is unset
    -- logical operators
    | TRU  | FLS            -- set status register to true or false respectively
    deriving (Show, Eq)
