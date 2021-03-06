#ifndef PREFSDK_DISASSEMBLERLISTING_H
#define PREFSDK_DISASSEMBLERLISTING_H

#include "prefsdk/type/datatype.h"
#include "prefsdk/type/datavalue.h"
#include "prefsdk/format/formattree.h"
#include "prefsdk/disassembler/blocks/segment.h"
#include "prefsdk/disassembler/blocks/label.h"
#include "prefsdk/disassembler/symbol/symboltable.h"
#include "prefsdk/disassembler/symbol/constanttable.h"
#include "prefsdk/disassembler/disassemblerdatabase.h"
#include "qhexedit/qhexeditdata.h"
#include "qhexedit/qhexeditdatareader.h"
#include <algorithm>
#include <QtCore>

namespace PrefSDK
{
    class DisassemblerListing : public QObject
    {
        Q_OBJECT

        Q_PROPERTY(PrefSDK::FormatTree* formattree READ formatTree)
        Q_PROPERTY(PrefSDK::SymbolTable* symboltable READ symbolTable)

        public:
            typedef QPair<Block*, QString> BookmarkEntry;
            typedef QMap<DataValue, Segment*> SegmentMap;
            typedef QMap<DataValue, Function*> FunctionMap;
            typedef QMap<DataValue, Instruction*> InstructionMap;
            typedef QMap<DataValue, Label*> LabelMap;
            typedef QList<Block*> BlockList;
            typedef QList<Function*> EntryPointList;
            typedef QList<BookmarkEntry> BookmarkList;

        public:
            explicit DisassemblerListing(QHexEditData* hexeditdata, Logger* logger, DataType::Type addresstype, QObject *parent = 0);
            void setFormatTree(PrefSDK::FormatTree* formattree);
            bool isAddress(const DataValue& address, Segment **outsegment = nullptr) const;
            bool isDecoded(const DataValue& address) const;
            void createFunction(const DataValue& address, const DataValue &calleraddress, FunctionType::Type functiontype, const QString& name);
            void createFunction(const DataValue& address, const DataValue &calleraddress, const QString& name);
            void createFunction(const DataValue& address, const DataValue &calleraddress);
            qint64 length() const;
            PrefSDK::FormatTree* formatTree();
            PrefSDK::SymbolTable* symbolTable();
            ConstantTable* constantTable();
            QHexEditData* data();
            const BlockList& blocks();
            DisassemblerListing::BookmarkList& bookmarks();
            const DisassemblerListing::SegmentMap& segments() const;
            const DisassemblerListing::FunctionMap& functions() const;
            const DisassemblerListing::EntryPointList& entryPoints() const;
            Segment* findSegment(Block* block);
            Segment* findSegment(const DataValue& address) const;
            Function* findFunction(Block* block);
            Function* findFunction(const DataValue& address);
            Instruction* findInstruction(const DataValue& address) const;
            Block* findBlock(const DataValue& address);
            void save(const QString& filename);

        public slots:
            void toggleBookmark(Block* block);
            void applyBookmark(Block* block, const QString& description);

        public:
            Q_INVOKABLE bool isAddress(lua_Integer address);
            Q_INVOKABLE void addInstruction(const PrefSDK::QtLua::LuaTable& instructiontable);
            Q_INVOKABLE void setFunction(lua_Integer address, const QString& name);
            Q_INVOKABLE void setFunction(lua_Integer address, lua_Integer functiontype, const QString& name);
            Q_INVOKABLE void createLabel(lua_Integer destaddress, lua_Integer calleraddress, const QString& name);
            Q_INVOKABLE void createSegment(const QString& name, lua_Integer segmenttype, lua_Integer startaddress, lua_Integer size, lua_Integer baseoffset);
            Q_INVOKABLE void createFunction(lua_Integer address, lua_Integer calleraddress, lua_Integer functiontype, const QString& name);
            Q_INVOKABLE void createFunction(lua_Integer address, lua_Integer calleraddress, const QString& name);
            Q_INVOKABLE void createFunction(lua_Integer address, lua_Integer calleraddress);
            Q_INVOKABLE void createEntryPoint(lua_Integer address, const QString& name);

        public: /* Modified Binary Search O(log(n) + 2k) */
            qint64 indexOf(Block* block);
            qint64 indexOf(const DataValue& address, Block::Type blocktype = Block::InstructionBlock);

        private:
            void checkSort();
            void removeInstructions(Instruction* from, Instruction* to);
            static bool sortBlocks(Block* block1, Block* block2);

        private:
            bool _blocksorted;
            QHexEditData* _hexeditdata;
            Logger* _logger;
            FormatTree* _formattree;
            DataType::Type _addresstype;
            SymbolTable* _symboltable;
            ConstantTable* _constanttable;
            EntryPointList _entrypoints;
            SegmentMap _segments;
            FunctionMap _functions;
            InstructionMap _instructions;
            LabelMap _labels;
            BlockList _blocks;
            BookmarkList _bookmarks;
    };
}

#endif // PREFSDK_DISASSEMBLERLISTING_H
