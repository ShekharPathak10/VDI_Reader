#ifndef EXT2_H
#define EXT2_H

struct Ext2SuperBlocks{
	uint32_t 	s_inodes_count;
	uint32_t	s_blocks_count;
	uint32_t	s_r_blocks_count;
	uint32_t	s_free_blocks_count;
	uint32_t	s_free_inodes_count;
	uint32_t	s_first_data_block;
	uint32_t	s_log_block_size;
	uint32_t	s_log_frag_size;
	uint32_t	s_blocks_per_group;
	uint32_t	s_frags_per_group;
	uint32_t	s_inodes_per_group;
	uint32_t	s_mtime;
	uint32_t	s_wtime;
	uint16_t	s_mnt_count;
	uint16_t	s_max_mnt_count;
	uint16_t	s_magic;
	uint16_t	s_state;
	uint16_t	s_errors;
	uint16_t	s_minor_rev_level;
	uint32_t	s_lastcheck;
	uint32_t	s_checkinterval;
	uint32_t	s_creator_os;
	uint32_t	s_rev_level;
	uint16_t	s_def_resuid;
	uint16_t	s_def_resgid;

	uint32_t	s_first_ino;
	uint16_t	s_inode_size;
	uint16_t	s_block_group_nr;
	uint32_t	s_feature_compat;
	uint32_t	s_feature_incompat;
	uint32_t	s_feature_ro_compat;
	uint32_t	s_uuid[4];
	uint32_t	s_volume_name[4];
	uint32_t	s_last_mounted[16];
	uint32_t	s_algorithm_bitmap;

	uint8_t		s_prealloc_blocks;
	uint8_t		s_prealloc_dir_blocks;
	uint16_t	alignment;

	uint32_t	s_journal_uuid[4];
	uint32_t	s_journal_inum;
	uint32_t	s_journal_dev;
	uint32_t	s_last_orphan;

	uint32_t	s_hash_seed[4];
	uint8_t		s_def_hash_version;
	uint8_t		padding[3];

	uint32_t	s_default_mount_op;
	uint32_t	s_first_meta_bg;
};

struct Ext2BlockGroupDescriptor{
	uint32_t bg_block_bitmap;
	uint32_t bg_inode_bitmap;
	uint32_t bg_inode_table;
	uint16_t bg_free_blocks;
	uint16_t bg_free_inodes;
	uint16_t bg_dirs_counts;
	uint16_t bg_pad;
	uint32_t bg_reserved[3];
};


struct Ext2File {
    VDIFile vdiFile;
    partitionFile partition_File;
    ssize_t numberofBlockGroups;
    Ext2SuperBlocks superBlocks;
    Ext2BlockGroupDescriptor * blockGroupDescriptorstable;
};

#endif /*EXT2.H*/





