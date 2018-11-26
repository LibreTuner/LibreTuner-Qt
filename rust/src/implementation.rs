#![allow(unused_imports)]
#![allow(unused_variables)]
#![allow(dead_code)]
use interface::*;

#[derive(Default, Clone)]
struct RomListItem {
    name: String,
}

pub struct RomList {
    emit: RomListEmitter,
    model: RomListList,
    list: Vec<RomListItem>,
}

impl RomListTrait for RomList {
    fn new(emit: RomListEmitter, model: RomListList) -> RomList {
        RomList {
            emit,
            model,
            list: Vec::new(),
        }
    }
    fn emit(&mut self) -> &mut RomListEmitter {
        &mut self.emit
    }
    fn row_count(&self) -> usize {
        self.list.len()
    }
    fn name(&self, index: usize) -> &str {
        &self.list[index].name
    }
}
pub struct Simple {
    emit: SimpleEmitter,
    message: String,
    rom_list: RomList,
}

impl SimpleTrait for Simple {
    fn new(emit: SimpleEmitter, rom_list: RomList) -> Simple {
        Simple {
            emit,
            message: "Hello, world!".to_string(),
            rom_list,
        }
    }
    fn emit(&mut self) -> &mut SimpleEmitter {
        &mut self.emit
    }
    fn message(&self) -> &str {
        &self.message
    }
    fn set_message(&mut self, value: String) {
        self.message = value;
        self.emit.message_changed();
    }
    fn rom_list(&self) -> &RomList {
        &self.rom_list
    }
    fn rom_list_mut(&mut self) -> &mut RomList {
        &mut self.rom_list
    }
}
