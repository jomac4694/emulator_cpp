
use aes_gcm_siv::{Aes256GcmSiv, Key, Nonce}; // Or `Aes128GcmSiv`
use aes_gcm_siv::aead::{Aead, NewAead};
use std::fs;
extern crate getopts;
use getopts::Options;
use std::env;
use std::path::Path;
use std::sync::mpsc::channel;
use std::thread;
use core::time::Duration;
use std::io::Write;
use std::net::TcpListener;
use std::io::Read;
//use protobuf::well_known_types::any::Any;
use protobuf::{EnumOrUnknown, Message, MessageFull};
include!(concat!(env!("OUT_DIR"), "/protos/mod.rs"));
use example::{get_response, GetRequest, GetResponse};
use std::collections::HashMap;
use std::collections::BTreeMap;
use std::collections::BTreeSet;
use std::marker::PhantomData;
use std::fmt::Display;
use std::any::TypeId;
use std::any::Any;
use std::fmt::Debug;
fn GetVal<T>(value: T) -> T
{
    return value;
}
fn read_file(file_path : &str) -> Vec<u8>
{
    println!("attempting to read file {}", file_path);
    let file_contents = fs::read(file_path).unwrap();
    return file_contents;
}

fn do_encrypt(plaintext: Vec<u8>, passwd: Vec<u8>) -> Vec<u8>
{
    let key = Key::from_slice(passwd.as_ref());
    let cipher = Aes256GcmSiv::new(key);

    let nonce = Nonce::from_slice(b"unique nonce"); // 96-bits; unique per message

    let ciphertext = cipher.encrypt(nonce, plaintext.as_ref())
        .expect("encryption failure!");  // NOTE: handle this error to avoid panics!


    let outty = cipher.decrypt(nonce, ciphertext.as_ref())
        .expect("decryption failure!");  // NOTE: handle this error to avoid panics!


    return ciphertext;
}

fn do_decrypt(ciphertext: Vec<u8>, passwd: Vec<u8>) -> Vec<u8>
{
    let key = Key::from_slice(passwd.as_ref());
    let cipher = Aes256GcmSiv::new(key);

    let nonce = Nonce::from_slice(b"unique nonce"); // 96-bits; unique per message

    let plaintext = cipher.decrypt(nonce, ciphertext.as_ref())
        .expect("decryption failure!");  // NOTE: handle this error to avoid panics!

    return plaintext;
}

fn encrypt_files(encrypt_path: &str, passwd: Vec<u8>)
{

    if Path::new(encrypt_path).is_file()
    {
        println!("is a file");
        let cipher = do_encrypt(read_file(encrypt_path), passwd);
        let s = String::from_utf8_lossy(&cipher);
        println!("the ct: {}", s);
        let encrypted_file = format!("{}_{}", encrypt_path, "encrypted");
        println!("encrypted: {}", encrypted_file);
        fs::write(encrypted_file, cipher).expect("Unable to write file");
    }
    else
    {
        println!("is not a file");
    }
    /*
    let paths = fs::read_dir("/Users/josephmcilvaine/").unwrap();

    for path in paths {
        let p = path.unwrap().path();
        let ext = p.extension();
        match ext {
            Some(value) => println!("fuck you {}", value.to_str().unwrap()),
            None => {}
        };

    }
    */
}


fn print_usage(program: &str, opts: Options) {
    let brief = format!("Usage: {} FILE [options]", program);
    print!("{}", opts.usage(&brief));
}

trait Runner: Send + Sync {
  //  type ReturnType: Send; // associated type
 //   type InputType: Send;
    fn run(&self, t: &str);
}



struct Calculation {  // <---- choose: name
    // <----- choose: inputs for your async work
}

impl Runner for Calculation {
   // type ReturnType = &'a str;  // <--- choose: calculation return type
   // type InputType: Send;
    fn run(&self, t: &str) {  // <-- implement: code executed by a thread
        println!("got a message from callback1: {}", t);
    }
}

struct Calculation2 {  // <---- choose: name
    // <----- choose: inputs for your async work
}

impl Runner for Calculation2 {
   // type ReturnType = &'a str;  // <--- choose: calculation return type
   // type InputType: Send;
    fn run(&self, t: &str) {  // <-- implement: code executed by a thread
        println!("got a message from callback2: {}", t);
    }
}

/*
struct ConnManager {
    callbacks: Vec<dyn Runner>,
}

impl ConnManager {
    pub fn new(name: String) -> Self {
        Self {
            callbacks: Vec::new(),
        }
    }
}
*/




trait Component<T: MessageFull>: Debug + Sized + MessageFull {

}

trait Storage<T: Debug>: Debug + MessageFull {
    fn new() -> Self
    where
        Self: Sized;

    fn insert(&mut self, value: T);
}


pub struct Callback<T>
where
    T: MessageFull
//    F: FnMut(Box<T>) + 'static,
{
    callback_map: HashMap<String, Box<dyn FnMut(T)>>,
    _phantom: PhantomData<T>,
}

impl<T> Callback<T>
where 
  //  F: FnMut(Box<T>) + 'static,
    T: MessageFull,
{
    pub fn new() -> Self{
        Callback {
            callback_map: HashMap::new(),
            _phantom: PhantomData {},
        }
    }

    pub fn RegisterCallback<F: FnMut(T) + 'static>(&mut self,  callback: F)
    {
     //   let desc = T::descriptor();
        self.callback_map.insert(String::from("balls"), Box::new(callback));
        print(5.0f32);
        print(5);
       // let dyn_msg = desc.new_instance();

     //   println!("type_url {}", any.unwrap().type_url);
       // if let Ok(m) = desc.new_instance().downcast::<T>() {
        //    println!("we gucci");
        //}
        //
    //    println!("registering {}", desc.name());
        //(&callback)(Box::new(b));
     //   match self.callback_map.get_mut(&String::from(desc.name())) {
      //      Some(mut funct) => (funct)(Box::new(b)),
      //      None => println!("fuck you")
      //  }
 
    }
}
use std::future::Future;
use std::pin::Pin;

type AsyncCallback = Box<dyn Fn() -> Pin<Box<dyn Future<Output = ()>>>>;


trait Evt<T: MessageFull, CB: FnMut(T)>
{
    type SelfType;
    new(call: CB) -> Self;
}

pub struct EvtCall<T, CB>
where 
T: MessageFull,
CB: FnMut(T)
{
    callb: CB,
    pd: PhantomData<T>,
}


/*
pub struct CMap<T, F>
where
    T: MessageFull,
    F: dyn EvtCall
{
    callbacks: HashMap<String, Box<F>>,
    pd: PhantomData<T>,
}
*/
async fn async_callback() {
    println!("Callback 2");
}


fn print<T: Display>(val: T)
{
    println!("print val {}", val);
}

fn HandleMsg(msg: GetRequest)
{
    println!("im being registered {}", &(*msg.name));
}

fn HandleResp(msg: GetResponse)
{
    println!("now im being registered");
}
fn main() {
   // let mut thingy = EvtCall::new(&HandleMsg);
   // let mut thingy2 = EvtCall::new(&HandleResp);
    //let mut m = GetRequest::new();
    //m.name = String::from("blah");
   // (thingy.callb)(m);
  //  map.insert("yeo", thingy);
  //  map.insert("yerrr", thingy2);
    //let mut mgr: StorageMgr = StorageMgr::new();

  //  mgr.get_storage_mut::<Pos>().insert(Pos(1.2, 3.4));
  //  mgr.get_storage_mut::<AudioVolume>().insert(AudioVolume(7));

  //  println!("{:?}", mgr.get_storage::<Pos>());
  //  println!("{:?}", mgr.get_storage::<AudioVolume>());
   // let mut serv = Callback::new();
   // serv.RegisterCallback(Box::new(|evt| HandleMsg(evt)));
  //  serv.RegisterCallback(Box::new(|evt| HandleMsg( evt)));
    
   // RegisterCallback::<GetRequest>(&HandleMsg);
/*
    let listener = TcpListener::bind("127.0.0.1:9123").unwrap();
    println!("listening started, ready to accept");
    for stream in listener.incoming() {
        thread::spawn(|| {
            let mut stream = stream.unwrap();
            stream.write(b"Hello World\r\n").unwrap();

            let mut line = String::with_capacity(512);
            let result = stream.read_to_string(&mut line);
            println!("yo");
            let thingy: &dyn Runner = &Calculation{};
            thingy.run(&line);
            let thingy2: &dyn Runner = &Calculation2{};
            thingy2.run(&line);
        });
    }
    */

    
    //let fut = do_async(&Calculation{ value: 12 });

    //let resp = fut().unwrap(); // call fut() to wait for the respbnse

   // println!("{}", resp);
/*
    // read_file("/Users/josephmcilvaine/rust_test.txt");
    let args: Vec<String> = env::args().collect();
    let program = args[0].clone();

    let mut opts = Options::new();
    opts.optopt("o", "", "File path", "NAME");
    opts.optflag("h", "help", "print this help menu");
    let matches = match opts.parse(&args[1..]) {
        Ok(m) => { m }
        Err(f) => { panic!("{}",f.to_string()) }
    };
    if matches.opt_present("h") {
        print_usage(&program, opts);
        return;
    }
    let output = matches.opt_str("o");

    let b = "00000000000000000000000000000000";
    let file_path = output.unwrap();
    encrypt_files(&file_path, b.as_bytes().to_vec());
*/
}
